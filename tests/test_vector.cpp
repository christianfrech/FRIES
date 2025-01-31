/*! \file
*
* \brief Tests related to indexing and storing elements in a vector
*/

#include "catch.hpp"
#include <cstring>
#include <FRIES/hh_vec.hpp>
#include <FRIES/Hamiltonians/hub_holstein.hpp>


TEST_CASE("Test insertion into sorted lists", "[ins_sorted]") {
    uint8_t src[] = {2, 4, 6, 8, 10};
    uint8_t dst[5];
    
    new_sorted(src, dst, 5, 2, 9);
    repl_sorted(src, 5, 2, 9);
    uint8_t res0[] = {2, 4, 8, 9, 10};
    for (size_t idx = 0; idx < 5; idx++) {
        REQUIRE(dst[idx] == res0[idx]);
        REQUIRE(src[idx] == res0[idx]);
    }
    src[2] = 6;
    src[3] = 8;
    
    res0[0] = 1;
    res0[1] = 2;
    res0[2] = 4;
    res0[3] = 8;
    new_sorted(src, dst, 5, 2, 1);
    repl_sorted(src, 5, 2, 1);
    for (size_t idx = 0; idx < 5; idx++) {
        REQUIRE(dst[idx] == res0[idx]);
        REQUIRE(src[idx] == res0[idx]);
    }
    src[0] = 2;
    src[1] = 4;
    src[2] = 6;
    
    res0[0] = 2;
    res0[1] = 4;
    res0[2] = 6;
    res0[4] = 12;
    new_sorted(src, dst, 5, 4, 12);
    repl_sorted(src, 5, 4, 12);
    for (size_t idx = 0; idx < 5; idx++) {
        REQUIRE(dst[idx] == res0[idx]);
        REQUIRE(src[idx] == res0[idx]);
    }
    src[4] = 10;
    
    res0[4] = 10;
    res0[2] = 7;
    new_sorted(src, dst, 5, 2, 7);
    repl_sorted(src, 5, 2, 7);
    for (size_t idx = 0; idx < 5; idx++) {
        REQUIRE(dst[idx] == res0[idx]);
        REQUIRE(src[idx] == res0[idx]);
    }
    src[2] = 6;
    
    res0[2] = 5;
    new_sorted(src, dst, 5, 2, 5);
    repl_sorted(src, 5, 2, 5);
    for (size_t idx = 0; idx < 5; idx++) {
        REQUIRE(dst[idx] == res0[idx]);
        REQUIRE(src[idx] == res0[idx]);
    }
}

TEST_CASE("Test generation of occupied lists from excitations", "[ex_occ]") {
    uint8_t n_elec = 10;
    uint8_t occ_orig[] = {2, 4, 6, 8, 10, 21, 23, 25, 27, 29};
    uint8_t occ_new[n_elec];
    uint8_t occ_result[] = {2, 4, 6, 8, 10, 21, 23, 25, 27, 29};
    
    uint8_t sing_ex[] = {2, 15};
    sing_ex_orbs(occ_orig, occ_new, sing_ex, n_elec);
    occ_result[2] = 8;
    occ_result[3] = 10;
    occ_result[4] = 15;
    for (size_t idx = 0; idx < n_elec; idx++) {
        REQUIRE(occ_new[idx] == occ_result[idx]);
    }
    occ_result[2] = 6;
    occ_result[3] = 8;
    occ_result[4] = 10;
    
    sing_ex[1] = 1;
    sing_ex_orbs(occ_orig, occ_new, sing_ex, n_elec);
    occ_result[0] = 1;
    occ_result[1] = 2;
    occ_result[2] = 4;
    for (size_t idx = 0; idx < n_elec; idx++) {
        REQUIRE(occ_new[idx] == occ_result[idx]);
    }
    occ_result[0] = 2;
    occ_result[1] = 4;
    occ_result[2] = 6;
    
    sing_ex[0] = 5;
    sing_ex[1] = 35;
    sing_ex_orbs(occ_orig, occ_new, sing_ex, n_elec);
    occ_result[5] = 23;
    occ_result[6] = 25;
    occ_result[7] = 27;
    occ_result[8] = 29;
    occ_result[9] = 35;
    for (size_t idx = 0; idx < n_elec; idx++) {
        REQUIRE(occ_new[idx] == occ_result[idx]);
    }
    occ_result[5] = 21;
    occ_result[6] = 23;
    occ_result[7] = 25;
    occ_result[8] = 27;
    occ_result[9] = 29;
    
    uint8_t doub_ex[] = {2, 4, 1, 19};
    occ_result[0] = 1;
    occ_result[1] = 2;
    occ_result[2] = 4;
    occ_result[4] = 19;
    doub_ex_orbs(occ_orig, occ_new, doub_ex, n_elec);
    for (size_t idx = 0; idx < n_elec; idx++) {
        REQUIRE(occ_new[idx] == occ_result[idx]);
    }
    occ_result[0] = 2;
    occ_result[1] = 4;
    occ_result[2] = 6;
    occ_result[4] = 10;
    
    doub_ex[1] = 7;
    doub_ex[2] = 7;
    doub_ex[3] = 22;
    occ_result[2] = 7;
    occ_result[6] = 22;
    occ_result[7] = 23;
    doub_ex_orbs(occ_orig, occ_new, doub_ex, n_elec);
    for (size_t idx = 0; idx < n_elec; idx++) {
        REQUIRE(occ_new[idx] == occ_result[idx]);
    }
    occ_result[2] = 6;
    occ_result[6] = 23;
    occ_result[7] = 25;
}


TEST_CASE("Test encoding and decoding of Holstein basis states", "[phonon_bits]") {
    uint8_t n_sites = 5;
    uint8_t ph_bits = 3;
    uint8_t det_size = 4;
    
    uint8_t det1[det_size];
    uint8_t ph_nums1[] = {5, 2, 0, 7, 1};
    uint8_t ph_nums2[n_sites];
    
    det1[1] = 0b01010100;
    det1[2] = 0b01111000;
    det1[3] = 0;
    
    std::vector<uint32_t> tmp;
    HubHolVec<int> sol_vec(1, 0, n_sites, ph_bits, 0, 1, NULL, 1, tmp, tmp);
    
    sol_vec.decode_phonons(det1, ph_nums2);
    
    for (size_t site_idx = 0; site_idx < n_sites; site_idx++) {
        REQUIRE(ph_nums2[site_idx] == ph_nums1[site_idx]);
    }
    
    uint8_t excite_det[det_size];
    sol_vec.det_from_ph(det1, excite_det, 2, 1);
    ph_nums1[2]++;
    sol_vec.decode_phonons(excite_det, ph_nums2);
    
    for (size_t site_idx = 0; site_idx < n_sites; site_idx++) {
        REQUIRE(ph_nums2[site_idx] == ph_nums1[site_idx]);
    }
    
    n_sites = 2;
    det_size = 2;
    
    det1[0] = 0b1001;
    det1[1] = 1;
    HubHolVec<int> sol_vec2(1, 0, n_sites, ph_bits, 2, 1, NULL, 1, tmp, tmp);
    sol_vec2.det_from_ph(det1, excite_det, 1, -1);
    
    REQUIRE(excite_det[0] == 0b10001001);
    REQUIRE(excite_det[1] == 0);
}


TEST_CASE("Test adding elements to vector", "[vector_add]") {
    uint8_t n_orb = 4;
    uint8_t n_elec = 2;
    uint8_t det_size = 1;
    uint8_t bit_str1[det_size];
    std::vector<uint32_t> proc_scrambler = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<uint32_t> vec_scrambler = {8, 7, 6, 5, 4, 3, 2, 1};
    
    DistVec<int> vec(2, 2, n_orb * 2, n_elec, 1, proc_scrambler, vec_scrambler);
    
    std::fill(bit_str1, bit_str1 + det_size, 255);
    gen_hf_bitstring(n_orb, n_elec, bit_str1);
    REQUIRE(bit_str1[0] == 0b00010001);
    
    vec.add(bit_str1, 1, 1);
    vec.perform_add(0);
    REQUIRE(*vec[0] == 1);
    REQUIRE(!memcmp(bit_str1, vec.indices()[0], det_size));
    
    vec.add(bit_str1, 1, 1);
    vec.perform_add(0);
    REQUIRE(*vec[0] == 2);
    
    bit_str1[0] = 0b00100001;
    vec.add(bit_str1, -1, 1);
    vec.perform_add(0);
    REQUIRE(*vec[1] == -1);
    REQUIRE(!memcmp(bit_str1, vec.indices()[1], det_size));
    
    vec.add(bit_str1, -1, 1);
    vec.perform_add(0);
    REQUIRE(*vec[1] == -2);
}
