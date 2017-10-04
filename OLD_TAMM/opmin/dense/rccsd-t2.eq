{
  range O = 10;
  range V = 100;

  index h1, h2, h3, h4 = O;
  index p1, p2, p3, p4 = V;

  array f_oo([O][O]);
  array f_ov([O][V]);
  array f_vo([V][O]);
  array f_vv([V][V]);
  array v_oooo([O,O][O,O]);
  array v_oovo([O,O][V,O]);
  array v_oovv([O,O][V,V]);
  array v_ovoo([O,V][O,O]);
  array v_ovvo([O,V][V,O]);
  array v_vooo([V,O][O,O]);
  array v_vovo([V,O][V,O]);
  array v_vovv([V,O][V,V]);
  array v_vvoo([V,V][O,O]);
  array v_vvvo([V,V][V,O]);
  array v_vvvv([V,V][V,V]);
  array t_vo([V][O]);
  array t_vvoo([V,V][O,O]);
  array r_vo([V][O]);
  array r_vvoo([V,V][O,O]);                 

  r_vvoo[p1,p2,h1,h2] =
    1.0 * v_vvoo[p1,p2,h1,h2]
    + 1.0 * f_vv[p1,p3] * t_vvoo[p3,p2,h1,h2]
    + 1.0 * f_vv[p2,p3] * t_vvoo[p1,p3,h1,h2]
    - 1.0 * f_oo[h3,h1] * t_vvoo[p2,p1,h2,h3]
    - 1.0 * f_oo[h3,h2] * t_vvoo[p1,p2,h1,h3]
    + 1.0 * v_vvvv[p1,p2,p3,p4] * t_vvoo[p3,p4,h1,h2]
    + 1.0 * v_vvvo[p1,p2,p3,h2] * t_vo[p3,h1]
    + 1.0 * v_vvvo[p2,p1,p3,h1] * t_vo[p3,h2]
    - 1.0 * v_vovo[p1,h3,p3,h1] * t_vvoo[p2,p3,h2,h3]
    - 1.0 * v_vovo[p2,h3,p3,h2] * t_vvoo[p1,p3,h1,h3]
    - 1.0 * v_vovo[p1,h3,p3,h2] * t_vvoo[p3,p2,h1,h3]
    - 1.0 * v_vovo[p2,h3,p3,h1] * t_vvoo[p3,p1,h2,h3]
    + 2.0 * v_ovvo[h3,p1,p3,h1] * t_vvoo[p2,p3,h2,h3]
    + 2.0 * v_ovvo[h3,p2,p3,h2] * t_vvoo[p1,p3,h1,h3]
    - 1.0 * v_ovvo[h3,p1,p3,h1] * t_vvoo[p3,p2,h2,h3]
    - 1.0 * v_ovvo[h3,p2,p3,h2] * t_vvoo[p3,p1,h1,h3]
    - 1.0 * v_ovoo[h3,p2,h1,h2] * t_vo[p1,h3]
    - 1.0 * v_vooo[p1,h3,h1,h2] * t_vo[p2,h3]
    + 1.0 * v_oooo[h3,h4,h1,h2] * t_vvoo[p1,p2,h3,h4]
    - 1.0 * f_ov[h3,p3] * t_vo[p3,h1] * t_vvoo[p2,p1,h2,h3]
    - 1.0 * f_ov[h3,p3] * t_vo[p3,h2] * t_vvoo[p1,p2,h1,h3]
    - 1.0 * f_ov[h3,p3] * t_vo[p1,h3] * t_vvoo[p3,p2,h1,h2]
    - 1.0 * f_ov[h3,p3] * t_vo[p2,h3] * t_vvoo[p1,p3,h1,h2]
    + 1.0 * v_vvvv[p1,p2,p3,p4] * t_vo[p3,h1] * t_vo[p4,h2]
    + 2.0 * v_vovv[p1,h3,p3,p4] * t_vo[p3,h1] * t_vvoo[p2,p4,h2,h3]
    + 2.0 * v_vovv[p2,h3,p3,p4] * t_vo[p3,h2] * t_vvoo[p1,p4,h1,h3]
    - 1.0 * v_vovv[p1,h3,p3,p4] * t_vo[p3,h1] * t_vvoo[p4,p2,h2,h3]
    - 1.0 * v_vovv[p2,h3,p3,p4] * t_vo[p3,h2] * t_vvoo[p4,p1,h1,h3]
    - 1.0 * v_vovv[p1,h3,p3,p4] * t_vo[p4,h1] * t_vvoo[p2,p3,h2,h3]
    - 1.0 * v_vovv[p2,h3,p3,p4] * t_vo[p4,h2] * t_vvoo[p1,p3,h1,h3]
    - 1.0 * v_vovv[p1,h3,p3,p4] * t_vo[p4,h2] * t_vvoo[p3,p2,h1,h3]
    - 1.0 * v_vovv[p2,h3,p3,p4] * t_vo[p4,h1] * t_vvoo[p3,p1,h2,h3]
    - 1.0 * v_vovv[p1,h3,p3,p4] * t_vo[p2,h3] * t_vvoo[p3,p4,h1,h2]
    - 1.0 * v_vovv[p2,h3,p3,p4] * t_vo[p1,h3] * t_vvoo[p4,p3,h1,h2]
    - 1.0 * v_vovv[p1,h3,p3,p4] * t_vo[p3,h3] * t_vvoo[p4,p2,h1,h2]
    - 1.0 * v_vovv[p2,h3,p3,p4] * t_vo[p3,h3] * t_vvoo[p1,p4,h1,h2]
    + 2.0 * v_vovv[p1,h3,p3,p4] * t_vo[p4,h3] * t_vvoo[p3,p2,h1,h2]
    + 2.0 * v_vovv[p2,h3,p3,p4] * t_vo[p4,h3] * t_vvoo[p1,p3,h1,h2]
    - 1.0 * v_vovo[p1,h3,p3,h2] * t_vo[p3,h1] * t_vo[p2,h3]
    - 1.0 * v_vovo[p2,h3,p3,h1] * t_vo[p3,h2] * t_vo[p1,h3]
    - 1.0 * v_ovvo[h3,p1,p3,h1] * t_vo[p3,h2] * t_vo[p2,h3]
    - 1.0 * v_ovvo[h3,p2,p3,h2] * t_vo[p3,h1] * t_vo[p1,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p3,p2,h1,h2] * t_vvoo[p1,p4,h4,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p3,h1,h2] * t_vvoo[p2,p4,h4,h3]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p3,p2,h1,h2] * t_vvoo[p1,p4,h3,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p3,h1,h2] * t_vvoo[p2,p4,h3,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p3,p4,h1,h3] * t_vvoo[p2,p1,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p2,h1,h3] * t_vvoo[p4,p3,h2,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p4,p3,h1,h3] * t_vvoo[p2,p1,h2,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p2,h1,h3] * t_vvoo[p3,p4,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p3,p4,h1,h2] * t_vvoo[p1,p2,h3,h4]
    + 4.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p3,h1,h3] * t_vvoo[p2,p4,h2,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p3,h1,h3] * t_vvoo[p4,p2,h2,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p3,p1,h1,h3] * t_vvoo[p2,p4,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p3,p1,h1,h3] * t_vvoo[p4,p2,h2,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p4,h1,h3] * t_vvoo[p2,p3,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p1,p4,h1,h3] * t_vvoo[p3,p2,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p4,p1,h1,h3] * t_vvoo[p2,p3,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vvoo[p4,p2,h1,h3] * t_vvoo[p3,p1,h2,h4]
    + 1.0 * v_oovo[h3,h4,p3,h1] * t_vo[p3,h2] * t_vvoo[p1,p2,h4,h3]
    + 1.0 * v_oovo[h3,h4,p3,h2] * t_vo[p3,h1] * t_vvoo[p1,p2,h3,h4]
    - 2.0 * v_oovo[h3,h4,p3,h1] * t_vo[p1,h4] * t_vvoo[p2,p3,h2,h3]
    - 2.0 * v_oovo[h3,h4,p3,h2] * t_vo[p2,h4] * t_vvoo[p1,p3,h1,h3]
    + 1.0 * v_oovo[h3,h4,p3,h1] * t_vo[p1,h4] * t_vvoo[p3,p2,h2,h3]
    + 1.0 * v_oovo[h3,h4,p3,h2] * t_vo[p2,h4] * t_vvoo[p3,p1,h1,h3]
    + 1.0 * v_oovo[h3,h4,p3,h1] * t_vo[p1,h3] * t_vvoo[p2,p3,h2,h4]
    + 1.0 * v_oovo[h3,h4,p3,h2] * t_vo[p2,h3] * t_vvoo[p1,p3,h1,h4]
    + 1.0 * v_oovo[h3,h4,p3,h1] * t_vo[p2,h3] * t_vvoo[p3,p1,h2,h4]
    + 1.0 * v_oovo[h3,h4,p3,h2] * t_vo[p1,h3] * t_vvoo[p3,p2,h1,h4]
    + 1.0 * v_oovo[h3,h4,p3,h1] * t_vo[p3,h4] * t_vvoo[p2,p1,h2,h3]
    + 1.0 * v_oovo[h3,h4,p3,h2] * t_vo[p3,h4] * t_vvoo[p1,p2,h1,h3]
    - 2.0 * v_oovo[h3,h4,p3,h1] * t_vo[p3,h3] * t_vvoo[p2,p1,h2,h4]
    - 2.0 * v_oovo[h3,h4,p3,h2] * t_vo[p3,h3] * t_vvoo[p1,p2,h1,h4]
    + 1.0 * v_oooo[h3,h4,h1,h2] * t_vo[p1,h3] * t_vo[p2,h4]
    - 1.0 * v_vovv[p1,h3,p3,p4] * t_vo[p3,h1] * t_vo[p4,h2] * t_vo[p2,h3]
    - 1.0 * v_vovv[p2,h3,p3,p4] * t_vo[p4,h1] * t_vo[p3,h2] * t_vo[p1,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p4,h2] * t_vvoo[p1,p2,h3,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p1,h3] * t_vvoo[p2,p4,h2,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h2] * t_vo[p2,h3] * t_vvoo[p1,p4,h1,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p1,h3] * t_vvoo[p4,p2,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h2] * t_vo[p2,h3] * t_vvoo[p4,p1,h1,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p1,h4] * t_vvoo[p2,p4,h2,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h2] * t_vo[p2,h4] * t_vvoo[p1,p4,h1,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p2,h4] * t_vvoo[p4,p1,h2,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h2] * t_vo[p1,h4] * t_vvoo[p4,p2,h1,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p4,h3] * t_vvoo[p2,p1,h2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h2] * t_vo[p4,h3] * t_vvoo[p1,p2,h1,h4]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p4,h4] * t_vvoo[p2,p1,h2,h3]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h2] * t_vo[p4,h4] * t_vvoo[p1,p2,h1,h3]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p1,h3] * t_vo[p2,h4] * t_vvoo[p3,p4,h1,h2]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p1,h3] * t_vo[p3,h4] * t_vvoo[p4,p2,h1,h2]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p2,h3] * t_vo[p3,h4] * t_vvoo[p1,p4,h1,h2]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vo[p1,h3] * t_vo[p4,h4] * t_vvoo[p3,p2,h1,h2]
    - 2.0 * v_oovv[h3,h4,p3,p4] * t_vo[p2,h3] * t_vo[p4,h4] * t_vvoo[p1,p3,h1,h2]
    + 1.0 * v_oovo[h3,h4,p3,h1] * t_vo[p3,h2] * t_vo[p2,h3] * t_vo[p1,h4]
    + 1.0 * v_oovo[h3,h4,p3,h2] * t_vo[p3,h1] * t_vo[p1,h3] * t_vo[p2,h4]
    + 1.0 * v_oovv[h3,h4,p3,p4] * t_vo[p3,h1] * t_vo[p4,h2] * t_vo[p1,h3] * t_vo[p2,h4];
}
