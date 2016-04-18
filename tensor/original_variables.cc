#include "variables.h"

namespace ctce {

  double Timer::total = 0.0;
  double Timer::dg_time = 0.0;
  double Timer::ah_time = 0.0;
  int Timer::dg_num = 0;
  int Timer::ah_num = 0;

  Integer Variables::noab_ = 0;
  Integer Variables::nvab_ = 0;
  Integer Variables::k_range_ = 0;
  Integer Variables::k_spin_ = 0;
  Integer Variables::k_sym_ = 0;
  Integer Variables::k_offset_ = 0;
  Integer Variables::k_evl_sorted_ = 0;
  Integer Variables::irrep_v_ = 0;
  Integer Variables::irrep_t_ = 0;
  Integer Variables::irrep_f_ = 0;
  logical Variables::intorb_ = 0;
  logical Variables::restricted_ = 0;
  Integer* Variables::int_mb_;
  double* Variables::dbl_mb_;

  void Variables::set_ov(Integer *o, Integer *v) {
    noab_ = *o;
    nvab_ = *v;
  }
  void Variables::set_idmb(Integer *int_mb_f, double *dbl_mb_f) {
    int_mb_ = int_mb_f - 1; 
    dbl_mb_ = dbl_mb_f - 1; 
  }
  void Variables::set_irrep(Integer *irrep_v, Integer *irrep_t, Integer *irrep_f) {
    irrep_v_ = *irrep_v; 
    irrep_t_ = *irrep_t;
    irrep_f_ = *irrep_f;
  }
  void Variables::set_k1(Integer *k_range, Integer *k_spin, Integer *k_sym) {
    k_range_ = *k_range;
    k_spin_ = *k_spin; 
    k_sym_ = *k_sym;
  }
  void Variables::set_k2(Integer *k_offset, Integer *k_evl_sorted) {
      k_offset_ = *k_offset;
      k_evl_sorted_ = *k_evl_sorted;
  }
  void Variables::set_log(logical *intorb, logical *restricted) {
    intorb_ = *intorb;
    restricted_ = *restricted;
  }

  extern "C" {

    // called in ccsd_t.F
    void set_k2_cxx_(Integer *k_offset, Integer *k_evl_sorted) {
      Variables::set_k2(k_offset, k_evl_sorted);
    }

    void set_var_cxx_(Integer* noab, Integer* nvab, 
        Integer* int_mb, double* dbl_mb,
        Integer* k_range, Integer *k_spin, Integer *k_sym, 
        logical *intorb, logical *restricted,
        Integer *irrep_v, Integer *irrep_t, Integer *irrep_f) {
      Dummy::construct();
      Table::construct();
      Variables::set_ov(noab, nvab);
      Variables::set_idmb(int_mb, dbl_mb);
      Variables::set_k1(k_range, k_spin, k_sym);
      Variables::set_log(intorb, restricted);
      Variables::set_irrep(irrep_v, irrep_t, irrep_f);
    }

    void showtimer_() {
      if (GA_Nodeid()==0) {
        printf("----- Node #%d -----\n DGEMM: %d\t%.10f\n ADDHB: %d\t%.10f\n TOTAL: %.10f\n",
          GA_Nodeid(), Timer::dg_num, Timer::dg_time, Timer::ah_num, Timer::ah_time, Timer::total);
      }
    }

  }

  std::vector<RangeType> Table::range_;
  std::vector<Integer> Table::value_;
  void Table::construct() {
    range_.resize(IndexNum);
    value_.resize(IndexNum);
    for (int i=0; i<pIndexNum; i++) range_[i] = TV;
    for (int i=pIndexNum; i<IndexNum; i++) range_[i] = TO;
  }

} /* namespace ctce */
