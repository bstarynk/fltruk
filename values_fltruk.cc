/****************************************************************
 * file fltruk/values_fltruk.cc
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Description:
 *      This file is the values file of fltruk
 *      © Copyright (C) 2026, Basile Starynkevitch, France
 *       <basile@starynkevitch.net>
 * It is GPLv3+ licensed (or CeCILL in France)
 *
 * Source code on https://github.com/bstarynk/fltruk
 *
 * License: GPLv3+ (file COPYING-GPLv3)
 *    This software is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/
#include "fltruk.hh"

extern "C" const char values_fltruk_git[] = FLTRUK_SHORTGIT;
extern "C" const char values_fltruk_base[] = FLTRUK_BASE;

extern "C" const uint64_t fltruk_primes_tab[];

static std::recursive_mutex fltruk_scalar_mtx;
static std::vector<std::list<fltruk_Value*>> fltruk_scalar_vect;
static std::atomic<unsigned> fltrk_nb_scalars;

static std::recursive_mutex fltruk_composite_mtx;
static std::vector<std::list<fltruk_Value*>> fltruk_composite_vect;
static std::atomic<unsigned> fltrk_nb_composites;


const uint64_t fltruk_primes_tab[] = {
    2, 3, 5, 7,
    11, 13, 17, 19,
    23, 29, 31, 37,
    41, 47, 53, 59,
    67, 73, 79, 89,
    97, 107, 127, 137,
    149, 163, 179, 191,
    211, 227, 251, 269,
    293, 313, 337, 367,
    397, 431, 461, 499,
    541, 587, 631, 677,
    727, 787, 853, 911,
    977, 1049, 1123, 1201,
    1283, 1373, 1471, 1571,
    1693, 1811, 1933, 2063,
    2203, 2351, 2521, 2693,
    2879, 3079, 3299, 3527,
    3767, 4019, 4289, 4583,
    4889, 5227, 5581, 5981,
    6389, 6823, 7283, 7789,
    8311, 8867, 9461, 10093,
    10771, 11491, 12263, 13093,
    13967, 14923, 15919, 16981,
    18119, 19333, 20627, 22003,
    23473, 25057, 26729, 28513,
//#100 of 3101 (p:28513)
    30427, 32467, 34649, 36973,
    39439, 42071, 44879, 47881,
    51109, 54517, 58153, 62039,
    66179, 70607, 75323, 80347,
    85711, 91433, 97547, 104053,
    111029, 118453, 126359, 134789,
    143779, 153371, 163601, 174527,
    186163, 198589, 211859, 225983,
    241049, 257123, 274271, 292561,
    312071, 332881, 355087, 378761,
    404017, 430957, 459691, 490339,
    523031, 557903, 595097, 634793,
    677113, 722257, 770417, 821801,
    876593, 935059, 997427, 1063927,
    1134863, 1210523, 1291229, 1377317,
    1469141, 1567087, 1671577, 1783037,
    1901917, 2028713, 2163971, 2308237,
    2462123, 2626277, 2801363, 2988143,
    3187397, 3399899, 3626561, 3868357,
    4126261, 4401347, 4694773, 5007791,
    5341649, 5697761, 6077629, 6482837,
    6915043, 7376051, 7867789, 8392327,
    8951819, 9548653, 10185257, 10864313,
    11588627, 12361211, 13185301, 14064371,
    15002017, 16002163, 17068981, 18206917,
//#200 of 1163623 (p:1.82069e+07)
    19420721, 20715473, 22096507, 23569613,
    25140923, 26816987, 28604809, 30511813,
    32545951, 34715693, 37030073, 39498757,
    42132017, 44940821, 47936887, 51132689,
    54541621, 58177747, 62056283, 66193423,
    70606351, 75313457, 80334391, 85690021,
    91402699, 97496219, 103996003, 110929111,
    118324391, 126212689, 134626873, 143602027,
    153175499, 163387199, 174279683, 185898353,
    198291617, 211511071, 225611821, 240652631,
    256696163, 273809243, 292063199, 311534141,
    332303099, 354456643, 378087103, 403292941,
    430179161, 458857793, 489448319, 522078229,
    556883447, 594009041, 633609649, 675850363,
    720907067, 768967541, 820232057, 874914199,
    933241831, 995457959, 1061821837, 1132609987,
    1208117321, 1288658479, 1374569071, 1466207011,
    1563954149, 1668217763, 1779432293, 1898061119,
    2024598539, 2159571809, 2303543269, 2457112831,
    2620920373, 2795648423, 2982025069, 3180826763,
    3392881901, 3619074031, 3860345639, 4117702019,
    4392215549, 4685029957, 4997365301, 5330523007,
    5685891217, 6064950649, 6469280711, 6900566093,
    7360603847, 7851310771, 8374731493, 8933046943,
    9528583469, 10163822369, 10841410537, 11564171261,
//#300 of 522761453 (p:1.15642e+10)
    12335116033, 13157457107, 14034621011, 14970262453,
    15968279959, 17032831973,
/// end, read 762939111 primes, printed 306 primes
    0
}; // end fltruk_primes_tab


int64_t
fltruk_prime_ranked (int rk)
{
    constexpr unsigned numprimes = sizeof (fltruk_primes_tab)
                                   / sizeof (fltruk_primes_tab[0]);
    if (rk < 0)
        return 0;
    if (rk < (int)numprimes)
        return fltruk_primes_tab[rk];
    return 0;
} // end of fltruk_prime_ranked

int64_t
fltruk_prime_above (uint64_t n)
{
    constexpr unsigned numprimes = sizeof (fltruk_primes_tab)
                                   / sizeof (fltruk_primes_tab[0]);
    int lo = 0, hi = numprimes;
    if (n >= fltruk_primes_tab[numprimes - 1])
        return 0;
    if (n < 2)
        return 2;
    while (lo + 8 < hi)
    {
        int md = (lo + hi) / 2;
        if (fltruk_primes_tab[md] > n)
            hi = md;
        else
            lo = md;
    };
    if (hi < (int) numprimes - 1)
        hi++;
    if (hi < (int) numprimes - 1)
        hi++;
    for (int ix = lo; ix < hi; ix++)
        if (fltruk_primes_tab[ix] > n)
            return fltruk_primes_tab[ix];
    return 0;
}

int64_t
fltruk_prime_greaterequal_ranked (uint64_t n, int*prank)
{
    constexpr unsigned numprimes = sizeof (fltruk_primes_tab)
                                   / sizeof (fltruk_primes_tab[0]);
    if (prank) *prank = -1;
    int lo = 0, hi = numprimes;
    if (n >= fltruk_primes_tab[numprimes - 1])
        return 0;
    if (n < 2)
        return 2;
    while (lo + 8 < hi)
    {
        int md = (lo + hi) / 2;
        if (fltruk_primes_tab[md] > n)
            hi = md;
        else
            lo = md;
    };
    if (hi < (int) numprimes - 1)
        hi++;
    if (hi < (int) numprimes - 1)
        hi++;
    for (int ix = lo; ix < hi; ix++)
        if (fltruk_primes_tab[ix] >= n)
        {
            if (prank)
                *prank =  ix;
            return fltruk_primes_tab[ix];
        }
    return 0;
} // end of fltruk_prime_greaterequal_ranked



int64_t
fltruk_prime_below (uint64_t n)
{
    constexpr unsigned numprimes =
        sizeof (fltruk_primes_tab) / sizeof (fltruk_primes_tab[0]);
    int lo = 0, hi = numprimes;
    if (n >= fltruk_primes_tab[numprimes - 1])
        return 0;
    if (n < 2)
        return 2;
    while (lo + 8 < hi)
    {
        int md = (lo + hi) / 2;
        if (fltruk_primes_tab[md] > n)
            hi = md;
        else
            lo = md;
    };
    if (hi < (int) numprimes - 1)
        hi++;
    if (hi < (int) numprimes - 1)
        hi++;
    for (int ix = hi; ix >= 0; ix--)
        if (fltruk_primes_tab[ix] < n)
            return fltruk_primes_tab[ix];
    return 0;
} // end fltruk_prime_below


int64_t
fltruk_prime_lessequal_ranked (uint64_t n, int*prank)
{
    constexpr unsigned numprimes =
        sizeof (fltruk_primes_tab) / sizeof (fltruk_primes_tab[0]);
    if (prank) *prank = -1;
    int lo = 0, hi = numprimes;
    if (n >= fltruk_primes_tab[numprimes - 1])
        return 0;
    if (n < 2)
        return 2;
    while (lo + 8 < hi)
    {
        int md = (lo + hi) / 2;
        if (fltruk_primes_tab[md] > n)
            hi = md;
        else
            lo = md;
    };
    if (hi < (int) numprimes - 1)
        hi++;
    if (hi < (int) numprimes - 1)
        hi++;
    for (int ix = hi; ix >= 0; ix--)
        if (fltruk_primes_tab[ix] <= n)
        {
            if (prank)
                *prank = ix;
            return fltruk_primes_tab[ix];
        }
    return 0;
} // end fltruk_prime_lessequal_ranked


fltruk_MethodDict fltruk_IntV::meth_int_dict("Int");

uint16_t
fltruk_IntV::register_int_val(struct fltruk_IntV*p, intptr_t i)
{
    // this is called by the constructor, whih is called by fltruk_make_int
    assert(p != nullptr && p->valtype() == fltruk_Vtype::IntTy);
    uint32_t h = fltruk_IntV::hash_int(i);
    assert (h>0);
    std::lock_guard gu(fltruk_scalar_mtx);
    size_t vlen = fltruk_scalar_vect.size();
    assert (vlen > 0 && vlen<USHRT_MAX);
    uint16_t ix = h % vlen;
    std::list<fltruk_Value*>& lis = fltruk_scalar_vect.at(ix);
#warning unimplemented fltruk_IntV::register_int_val
    FLTRUK_VERBOSE("in register_int_val p@%p i=%ld",
                   (void*)p, (long)i);
    asm volatile("nop; nop; nop; nop");
    fflush(nullptr);
    throw std::runtime_error("register_int_val");
} // end fltruk_IntV::register_int_val

const fltruk_IntV*
fltruk_make_int(intptr_t i)
{
    fltruk_IntV*res = nullptr;
    uint32_t h = fltruk_IntV::hash_int(i);
    assert (h > 0);
    std::lock_guard gu(fltruk_scalar_mtx);
    size_t vlen = fltruk_scalar_vect.size();
    assert (vlen > 0 && vlen<USHRT_MAX);
    std::list<fltruk_Value*>& lis = fltruk_scalar_vect.at(h % vlen);
    for (auto it: lis) {
        const fltruk_Value* p = it;
        assert (p != nullptr);
        if (p->valtype() == fltruk_Vtype::IntTy) {
            const fltruk_IntV* pi = reinterpret_cast<const fltruk_IntV*>(p);
            if (pi->int_val() == i)
                return pi;
        }
    }
    res = new fltruk_IntV(i);
    lis.push_back(res);
    fltrk_nb_scalars.fetch_add(1);
    res->_vnum = h % vlen;
    return res;
} // end fltruk_make_int

fltruk_MethodDict fltruk_DoubleV::meth_double_dict("Double");

uint16_t
fltruk_DoubleV::register_double_val(struct fltruk_DoubleV*p, double x)
{
    assert(p != nullptr && p->valtype() == fltruk_Vtype::DoubleTy);
#warning unimplemented fltruk_DoubleV::register_double_val
    FLTRUK_VERBOSE("in register_double_val pd@%p x=%g",
                   (void*)p, x);
    asm volatile("nop; nop; nop; nop");
    fflush(nullptr);
    throw std::runtime_error("register_double_val");
} // end fltruk_DoubleV::register_double_val




fltruk_MethodDict fltruk_StringV::meth_string_dict("String");

uint16_t
fltruk_StringV::register_string_val(struct fltruk_StringV*p, const std::string s)
{
    assert(p != nullptr && p->valtype() == fltruk_Vtype::StringTy);
#warning unimplemented fltruk_StringV::register_string_val
    FLTRUK_VERBOSE("in register_string_val pd@%p s='%s'",
                   (void*)p, s.c_str());
    asm volatile("nop; nop; nop; nop");
    fflush(nullptr);
    throw std::runtime_error("register_string_val");
} // end fltruk_StringV::register_string_val


uint32_t
fltruk_StringV::hash_string(const std::string str)
{
    uint32_t h=0;
    const char* ps = str.c_str();
    size_t bytsiz = str.size();
    const char* pend = ps + bytsiz;
    if (u8_check ((const uint8_t*)ps, bytsiz))
    {
        FLTRUK_WARNING("bad (non UTF8) string %s", ps);
        return 0;
    };
    while (ps && ps < pend && *ps) {
        ucs4_t uc = 0xfffd;
        int nb = u8_mbtouc (&uc, (const uint8_t*)ps, pend-ps);
        if (nb<0 || uc==0xffffd)
            return 0;
        assert (nb>0);
        h = (349 * uc) ^ (h+11+7*(pend - ps));
        ps += nb;
    };
    if (h == 0)
        h = bytsiz+1;
    return h;
} // end fltruk_StringV::hash_string

extern "C" fltruk_marksig_t fltruk_mark_scalar;
constexpr unsigned fltruk_nb_scalar_mtx = 256;
extern "C" std::recursive_mutex fltruk_scalar_array_mtx[fltruk_nb_scalar_mtx];
std::recursive_mutex fltruk_scalar_array_mtx[fltruk_nb_scalar_mtx];
bool
fltruk_mark_scalar(struct fltruk_Value* val, struct fltruk_GarbCollData*gcd)
{
    assert (val);
    assert (gcd);
    uint16_t vnum = val->valnum();
    assert (vnum < fltruk_nb_scalar_mtx);
    std::lock_guard<std::recursive_mutex> gu(fltruk_scalar_array_mtx[vnum]);
    uint8_t vgc = val->gc_fetch_mark_or(1);
    return vgc | 1;
} // end fltruk_mark_scalar

void
fltruk_initialize_values(void)
{
    static bool inited;
    assert (!inited);
    fltruk_scalar_vect.reserve(4096);
    fltruk_composite_vect.reserve(2048);
#warning incomplete fltruk_initialize_values
    FLTRUK_WARNING("incomplete fltruk_initialize_valuesé");
    inited = true;
} // end fltruk_initialize_values

/********
 ** for GNU emacs...
 ** Local-Variables: ;;
 ** compile-command: "make -j3" ;;
 ** End: ;;
 *********/
// end of file fltruk/values_fltruk.cc
