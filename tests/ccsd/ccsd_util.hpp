

#include "cd_svd.hpp"
#include "macdecls.h"
#include "ga-mpi.h"


using namespace tamm;
using Tensor2D   = Eigen::Tensor<double, 2, Eigen::RowMajor>;
using Tensor3D   = Eigen::Tensor<double, 3, Eigen::RowMajor>;
using Tensor4D   = Eigen::Tensor<double, 4, Eigen::RowMajor>;


  auto lambdar2 = [&](const IndexVector& blockid, span<double> buf){
      if((blockid[0] > blockid[1]) || (blockid[2] > blockid[3])) {
          for(auto i = 0U; i < buf.size(); i++) buf[i] = 0; 
      }
  };

std::string ccsd_test( int argc, char* argv[] )
{

    if(argc<2){
        std::cout << "Please provide an input file!\n";
        exit(0);
    }

    auto filename = std::string(argv[1]);
    std::ifstream testinput(filename); 
    if(!testinput){
        std::cout << "Input file provided [" << filename << "] does not exist!\n";
        exit(0);
    }

    return filename;
}

void iteration_print(const ProcGroup& pg, int iter, double residual, double energy, double time) {
  if(pg.rank() == 0) {
    std::cout.width(6); std::cout << std::right << iter+1 << "  ";
    std::cout << std::setprecision(13) << residual << "  ";
    std::cout << std::fixed << std::setprecision(13) << energy << " ";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(4, ' ') << "0.0";
    std::cout << std::string(5, ' ') << time;
    std::cout << std::string(5, ' ') << "0.0" << std::endl;
  }
}

void iteration_print_lambda(const ProcGroup& pg, int iter, double residual, double time) {
  if(pg.rank() == 0) {
    std::cout.width(6); std::cout << std::right << iter+1 << "  ";
    std::cout << std::setprecision(13) << residual << "  ";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(8, ' ') << "0.0";
    std::cout << std::string(5, ' ') << time << std::endl;
  }
}

/**
 *
 * @tparam T
 * @param MO
 * @param p_evl_sorted
 * @return pair of residual and energy
 */
template<typename T>
std::pair<double,double> rest(ExecutionContext& ec,
                              const TiledIndexSpace& MO,
                               Tensor<T>& d_r1,
                               Tensor<T>& d_r2,
                               Tensor<T>& d_t1,
                               Tensor<T>& d_t2,
                               Tensor<T>& de,
                              std::vector<T>& p_evl_sorted, T zshiftl, 
                              const TAMM_SIZE& noab,bool transpose=false) {

    T residual, energy;
    Scheduler sch{ec};
    Tensor<T> d_r1_residual{}, d_r2_residual{};
    Tensor<T>::allocate(&ec,d_r1_residual, d_r2_residual);
    sch
      (d_r1_residual() = d_r1()  * d_r1())
      (d_r2_residual() = d_r2()  * d_r2())
      .execute();

      auto l0 = [&]() {
        T r1 = get_scalar(d_r1_residual);
        T r2 = get_scalar(d_r2_residual);
        r1 = 0.5*std::sqrt(r1);
        r2 = 0.5*std::sqrt(r2);
        energy = get_scalar(de);
        residual = std::max(r1,r2);
      };

      auto l1 =  [&]() {
        jacobi(ec, d_r1, d_t1, -1.0 * zshiftl, transpose, p_evl_sorted,noab);
      };
      auto l2 = [&]() {
        jacobi(ec, d_r2, d_t2, -2.0 * zshiftl, transpose, p_evl_sorted,noab);
      };

      l0();
      l1();
      l2();

      Tensor<T>::deallocate(d_r1_residual, d_r2_residual);
      
    return {residual, energy};
}

std::tuple<TiledIndexSpace,TAMM_SIZE> setupMOIS(TAMM_SIZE nao, TAMM_SIZE ov_alpha, TAMM_SIZE freeze_core, TAMM_SIZE freeze_virtual){

    TAMM_SIZE ov_beta{nao - ov_alpha};

    // std::cout << "ov_alpha,nao === " << ov_alpha << ":" << nao << std::endl;
    std::vector<TAMM_SIZE> sizes = {ov_alpha - freeze_core, ov_alpha - freeze_core,
             ov_beta - freeze_virtual, ov_beta - freeze_virtual};

    const TAMM_SIZE total_orbitals = 2*ov_alpha+2*ov_beta - 2 * freeze_core - 2 * freeze_virtual;
    
    // Construction of tiled index space MO
    IndexSpace MO_IS{range(0, total_orbitals),
                    {
                     {"occ", {range(0, 2*ov_alpha)}},
                     {"virt", {range(2*ov_alpha, total_orbitals)}}
                    },
                     { 
                      {Spin{1}, {range(0, ov_alpha), range(2*ov_alpha,2*ov_alpha+ov_beta)}},
                      {Spin{2}, {range(ov_alpha, 2*ov_alpha), range(2*ov_alpha+ov_beta, total_orbitals)}} 
                     }
                     };

    // IndexSpace MO_IS{range(0, total_orbitals),
    //                 {{"occ", {range(0, ov_alpha+ov_beta)}}, //0-7
    //                  {"virt", {range(total_orbitals/2, total_orbitals)}}, //7-14
    //                  {"alpha", {range(0, ov_alpha),range(ov_alpha+ov_beta,2*ov_alpha+ov_beta)}}, //0-5,7-12
    //                  {"beta", {range(ov_alpha,ov_alpha+ov_beta), range(2*ov_alpha+ov_beta,total_orbitals)}} //5-7,12-14   
    //                  }};
    const unsigned int ova = static_cast<unsigned int>(ov_alpha);
    const unsigned int ovb = static_cast<unsigned int>(ov_beta);
    TiledIndexSpace MO{MO_IS, {ova,ova,ovb,ovb}};

    return std::make_tuple(MO,total_orbitals);
}

template<typename T>
std::tuple<std::vector<T>, Tensor<T>,Tensor<T>,Tensor<T>,Tensor<T>,
std::vector<Tensor<T>>,std::vector<Tensor<T>>,std::vector<Tensor<T>>,std::vector<Tensor<T>>>
 setupTensors(ExecutionContext& ec, TiledIndexSpace& MO, Tensor<T> d_f1, size_t ndiis) {

    auto rank = ec.pg().rank();

    TiledIndexSpace O = MO("occ");
    TiledIndexSpace V = MO("virt");
 
    std::vector<T> p_evl_sorted = tamm::diagonal(ec,d_f1());

    auto lambda2 = [&](const IndexVector& blockid, span<T> buf){
        if(blockid[0] != blockid[1]) {
            for(auto i = 0U; i < buf.size(); i++) buf[i] = 0; 
        }
    };

    update_tensor(d_f1(),lambda2);

 if(rank == 0) {
    std::cout << "\n\n";
    std::cout << " CCSD iterations" << std::endl;
    std::cout << std::string(66, '-') << std::endl;
    std::cout <<
        " Iter          Residuum       Correlation     Cpu    Wall    V2*C2"
              << std::endl;
    std::cout << std::string(66, '-') << std::endl;
  }
   
  std::vector<Tensor<T>> d_r1s, d_r2s, d_t1s, d_t2s;

  for(int i=0; i<ndiis; i++) {
    d_r1s.push_back(Tensor<T>{{V,O},{1,1}});
    d_r2s.push_back(Tensor<T>{{V,V,O,O},{2,2}});
    d_t1s.push_back(Tensor<T>{{V,O},{1,1}});
    d_t2s.push_back(Tensor<T>{{V,V,O,O},{2,2}});
    Tensor<T>::allocate(&ec,d_r1s[i], d_r2s[i], d_t1s[i], d_t2s[i]);
  }

  Tensor<T> d_t1{{V,O},{1,1}};
  Tensor<T> d_t2{{V,V,O,O},{2,2}};
  Tensor<T> d_r1{{V,O},{1,1}};
  Tensor<T> d_r2{{V,V,O,O},{2,2}};
  Tensor<T>::allocate(&ec,d_r1,d_r2,d_t1,d_t2);

  Scheduler{ec}   
  (d_r1() = 0)
  (d_r2() = 0)
  (d_t1() = 0)
  (d_t2() = 0)
  .execute();

  return std::make_tuple(p_evl_sorted,d_t1,d_t2,d_r1,d_r2, d_r1s, d_r2s, d_t1s, d_t2s);
}

template<typename T>
std::tuple<TAMM_SIZE, TAMM_SIZE, double, libint2::BasisSet, std::vector<size_t>, Tensor<T>, Tensor<T>, TiledIndexSpace, TiledIndexSpace> 
    hartree_fock_driver(ExecutionContext &ec, const string filename) {

    auto rank = ec.pg().rank();
    TAMM_SIZE ov_alpha{0};
    double hf_energy{0.0};
    libint2::BasisSet shells;
    TAMM_SIZE nao{0};
    Tensor<T> C_AO;
    Tensor<T> F_AO;
    TiledIndexSpace tAO; //Fixed Tilesize AO
    TiledIndexSpace tAOt; //original AO TIS
    std::vector<size_t> shell_tile_map;

    auto hf_t1 = std::chrono::high_resolution_clock::now();

    std::tie(ov_alpha, nao, hf_energy, shells, shell_tile_map, C_AO, F_AO, tAO, tAOt) = hartree_fock(ec, filename);
    auto hf_t2 = std::chrono::high_resolution_clock::now();

    double hf_time =
      std::chrono::duration_cast<std::chrono::duration<double>>((hf_t2 - hf_t1)).count();
    if(rank == 0) std::cout << "\nTime taken for Hartree-Fock: " << hf_time << " secs\n";

    return std::make_tuple(ov_alpha, nao, hf_energy, shells, shell_tile_map, C_AO, F_AO, tAO, tAOt);
}


template<typename T> 
std::tuple<Tensor<T>,Tensor<T>,TAMM_SIZE, tamm::Tile>  cd_svd_driver(ExecutionContext& ec, TiledIndexSpace& MO,
    TiledIndexSpace& AO_tis,
  const TAMM_SIZE ov_alpha, const TAMM_SIZE nao, const TAMM_SIZE freeze_core,
  const TAMM_SIZE freeze_virtual, Tensor<TensorType> C_AO, Tensor<TensorType> F_AO,
  libint2::BasisSet& shells, std::vector<size_t>& shell_tile_map){


    tamm::Tile max_cvecs = 8*nao;
    auto diagtol = 1.0e-6; // tolerance for the max. diagonal

    auto rank = ec.pg().rank();
    TiledIndexSpace N = MO("all");

    Tensor<T> d_f1{{N,N},{1,1}};
    Tensor<T>::allocate(&ec,d_f1);

    auto hf_t1        = std::chrono::high_resolution_clock::now();
    TAMM_SIZE chol_count = 0;

    //std::tie(V2) = 
    Tensor<T> cholVpr = cd_svd(ec, MO, AO_tis, ov_alpha, nao, freeze_core, freeze_virtual,
                                C_AO, F_AO, d_f1, chol_count, max_cvecs, diagtol, shells, shell_tile_map);
    auto hf_t2        = std::chrono::high_resolution_clock::now();
    double cd_svd_time =
      std::chrono::duration_cast<std::chrono::duration<double>>((hf_t2 - hf_t1)).count();

    if(rank == 0) std::cout << "\nTotal Time taken for CD (+SVD): " << cd_svd_time
              << " secs\n";

    Tensor<T>::deallocate(C_AO,F_AO);

    // IndexSpace CI{range(0, max_cvecs)};
    // TiledIndexSpace tCI{CI, max_cvecs};
    // auto [cindex] = tCI.labels<1>("all");

    // IndexSpace CIp{range(0, chol_count)};
    // TiledIndexSpace tCIp{CIp, 1};
    // auto [cindexp] = tCIp.labels<1>("all");

    return std::make_tuple(cholVpr, d_f1, chol_count, max_cvecs);

}

void ccsd_stats(ExecutionContext& ec, double hf_energy,double residual,double energy,double thresh){

    auto rank = ec.pg().rank();
      if(rank == 0) {
    std::cout << std::string(66, '-') << std::endl;
    if(residual < thresh) {
        std::cout << " Iterations converged" << std::endl;
        std::cout.precision(15);
        std::cout << " CCSD correlation energy / hartree ="
                  << std::setw(26) << std::right << energy
                  << std::endl;
        std::cout << " CCSD total energy / hartree       ="
                  << std::setw(26) << std::right
                  << energy + hf_energy << std::endl;
    }
  }

}

// template<typename T> 
// void freeTensors(size_t ndiis, Tensor<T>& d_r1, Tensor<T>& d_r2, Tensor<T>& d_t1, Tensor<T>& d_t2,
//                    Tensor<T>& d_f1, std::vector<Tensor<T>>& d_r1s, 
//                    std::vector<Tensor<T>>& d_r2s, std::vector<Tensor<T>>& d_t1s, 
//                    std::vector<Tensor<T>>& d_t2s) {

//   for(auto i=0; i<ndiis; i++)
//     Tensor<T>::deallocate(d_r1s[i], d_r2s[i], d_t1s[i], d_t2s[i]);
//   d_r1s.clear(); d_r2s.clear();
//   d_t1s.clear(); d_t2s.clear();
//   Tensor<T>::deallocate(d_r1, d_r2, d_t1, d_t2, d_f1);//, d_v2);
// }


  auto free_vec_tensors = [&](auto&&... vecx) {
      (std::for_each(vecx.begin(), vecx.end(), [](auto& t) { t.deallocate(); }),
       ...);
  };

  auto free_tensors = [&](auto&&... t) {
      ( (t.deallocate()), ...);
  };


template<typename T>
std::tuple<Tensor<T>,Tensor<T>,Tensor<T>,Tensor<T>,std::vector<Tensor<T>>,std::vector<Tensor<T>>,
std::vector<Tensor<T>>,std::vector<Tensor<T>>> 
setupLambdaTensors(ExecutionContext& ec, TiledIndexSpace& MO, size_t ndiis) {

    TiledIndexSpace O = MO("occ");
    TiledIndexSpace V = MO("virt");
    
     auto rank = ec.pg().rank();

  Tensor<T> d_r1{{O,V},{1,1}};
  Tensor<T> d_r2{{O,O,V,V},{2,2}};
    Tensor<T> d_y1{{O,V},{1,1}};
    Tensor<T> d_y2{{O,O,V,V},{2,2}};

    Tensor<T>::allocate(&ec,d_r1, d_r2,d_y1,d_y2);

  Scheduler{ec}
      (d_y1() = 0)
      (d_y2() = 0)      
      (d_r1() = 0)
      (d_r2() = 0)
    .execute();

  if(rank == 0) {
    std::cout << "\n\n";
    std::cout << " Lambda CCSD iterations" << std::endl;
    std::cout << std::string(44, '-') << std::endl;
    std::cout <<
        " Iter          Residuum          Cpu    Wall"
              << std::endl;
    std::cout << std::string(44, '-') << std::endl;
  }

  std::vector<Tensor<T>> d_r1s,d_r2s,d_y1s, d_y2s;

  for(size_t i=0; i<ndiis; i++) {
    d_r1s.push_back(Tensor<T>{{O,V},{1,1}});
    d_r2s.push_back(Tensor<T>{{O,O,V,V},{2,2}});

    d_y1s.push_back(Tensor<T>{{O,V},{1,1}});
    d_y2s.push_back(Tensor<T>{{O,O,V,V},{2,2}});
    Tensor<T>::allocate(&ec,d_r1s[i],d_r2s[i],d_y1s[i], d_y2s[i]);
  }

    return std::make_tuple(d_r1,d_r2,d_y1,d_y2,d_r1s,d_r2s,d_y1s,d_y2s);

}


template<typename T>
Tensor<T> setupV2(ExecutionContext& ec, TiledIndexSpace& MO, Tensor<T> cholVpr, const tamm::Tile chol_count,
                 const TAMM_SIZE total_orbitals, TAMM_SIZE n_alpha, TAMM_SIZE n_beta) {

    auto rank = ec.pg().rank();

    TiledIndexSpace N = MO("all");

    IndexSpace CI{range(0, chol_count)};
    TiledIndexSpace tCI{CI, chol_count};
    auto [cindex] = tCI.labels<1>("all");

    auto [p,q,r,s] = MO.labels<4>("all");

    //Spin here is defined as spin(p)=spin(q) and spin(r)=spin(s) which is not currently not supported by TAMM.
    Tensor<T> d_a2{N,N,N,N};
    //For V2, spin(p)+spin(q) == spin(r)+spin(s)
    Tensor<T> d_v2{{N,N,N,N},{2,2}};
    Tensor<T>::allocate(&ec,d_a2,d_v2);

    Scheduler{ec}(d_a2(p, r, q, s) = cholVpr(cindex, p, r) * cholVpr(cindex, q, s)).execute();

    Scheduler{ec}(d_v2(p, q, r, s) = d_a2(p,r,q,s))
                  (d_v2(p, q, r, s) -= d_a2(p,s,q,r))
                  .execute();

    Tensor<T>::deallocate(d_a2);
    return d_v2;

 #if 0
      auto chol_dims = CholVpr.dimensions();
  auto chol_count = chol_dims[2];
    auto ndocc = ov_alpha;
    auto ov_alpha_freeze = ndocc - freeze_core;
    auto ov_beta_freeze  = nao - ndocc - freeze_virtual;
  const int n_alpha = ov_alpha_freeze;
  const int n_beta = ov_beta_freeze;
  // buf[0] points to the target shell set after every call  to engine.compute()
  // const auto &buf = engine.results();
  Matrix spin_t = Matrix::Zero(1, 2 * nao - 2 * freeze_core - 2 * freeze_virtual);
  Matrix spin_1 = Matrix::Ones(1,n_alpha);
  Matrix spin_2 = Matrix::Constant(1,n_alpha,2);
  Matrix spin_3 = Matrix::Constant(1,n_beta,1);
  Matrix spin_4 = Matrix::Constant(1,n_beta,2);
  //spin_t << 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2, 2; - water
  spin_t.block(0,0,1,n_alpha) = spin_1;
  spin_t.block(0,n_alpha,1,n_alpha) = spin_2;
  spin_t.block(0,2*n_alpha,1, n_beta) = spin_3;
  spin_t.block(0,2*n_alpha+n_beta,1, n_beta) = spin_4;

  const auto v2dim =  2 * nao - 2 * freeze_core - 2 * freeze_virtual;
    Tensor4D A2(v2dim,v2dim,v2dim,v2dim);
    A2.setZero();
    Tensor4D V2(v2dim,v2dim,v2dim,v2dim);

        // Form (pr|qs)
    for (auto p = 0; p < v2dim; p++) {
      for (auto r = 0; r < v2dim; r++) {
        if (spin_t(p) != spin_t(r)) {
          continue;
        }

        for (auto q = 0; q < v2dim; q++) {
          for (auto s = 0; s < v2dim; s++) {
            if (spin_t(q) != spin_t(s)) {
              continue;
            }

            for (auto icount = 0; icount != chol_count; ++icount) {
              A2(p, r, q, s) += CholVpr(p, r, icount) * CholVpr(q, s, icount);
              //V2_FromCholV(p, r, q, s) += CholVpr(p, r, icount) * CholVpr(q, s, icount);
            }
            //cout << p << " " << r << " " << q << " " << s << " " << V2_unfused(p, r, q, s) << "\n" << endl;
          }
        }
      }
    }


    for (size_t p = 0; p < v2dim; p++) {
        for (size_t q = 0; q < v2dim; q++) {
          for (size_t r = 0; r < v2dim; r++) {
            for (size_t s = 0; s < v2dim; s++) {
              V2(p, q, r, s) = A2(p, r, q, s) - A2(p, s, q, r);
            }
          }
        }
    }
 #endif

}
