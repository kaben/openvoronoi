#include <iostream>
#include <limits>
#include <string>

#include "common/numeric.hpp"
#include "version.hpp"

using namespace std;


// Convenience macro to output function name, file name, line number, and
// expression when expression evaluates to false. When false, increments
// fail-count of status argument "s".
#define check(expr, s) do { \
  if(!(expr)) { \
    s.fail(); \
    cout<<__func__<<":" __FILE__ ":"<<__LINE__<<": "<< #expr <<endl; \
  } else { s.pass(); } \
} while (0)

// Convenience class to track checks and fails during testing.
struct Status {
    unsigned int cases, errors;
    Status();
    void pass();
    void fail();
    void operator +=(const Status& s);
};
Status::Status(): cases(0), errors(0) {}
void Status::pass() { cases++; }
void Status::fail() { cases++; errors++; }
void Status::operator +=(const Status& s) {
    cases += s.cases;
    errors += s.errors;
}


// Test cases to verify adding things.
template <class Scalar>
Status test_specialized_Ac_add() {
    Status s;
    { // Verify behavior when adding positive scalar.
        ovd::numeric::Ac<Scalar> a;
        a.add(3);
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify positive accumulator was used.
        check(3 == a.p[0], s);
    }{ // Verify behavior when adding negative scalar.
        ovd::numeric::Ac<Scalar> a;
        a.add(-3);
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify negative accumulator was used.
        check(-3 == a.n[0], s);
    }
    return s;
}
Status test_Ac_add() {
    Status s;
    s += test_specialized_Ac_add<float>();
    s += test_specialized_Ac_add<double>();
    s += test_specialized_Ac_add<long double>();
    s += test_specialized_Ac_add<qd_real>();
    return s;
}


// Test cases to verify adding Scalar via +=.
template <class Scalar>
Status test_specialized_Ac_scalar_plus_eq() {
    Status s;
    { // Verify behavior when adding positive scalar.
        ovd::numeric::Ac<Scalar> a;
        a+=3;
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify positive accumulator was used.
        check(3 == a.p[0], s);
    }{ // Verify behavior when adding negative scalar.
        ovd::numeric::Ac<Scalar> a;
        a+=-3;
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify negative accumulator was used.
        check(-3 == a.n[0], s);
    }
    return s;
}
Status test_Ac_scalar_plus_eq() {
    Status s;
    s += test_specialized_Ac_scalar_plus_eq<float>();
    s += test_specialized_Ac_scalar_plus_eq<double>();
    s += test_specialized_Ac_scalar_plus_eq<long double>();
    s += test_specialized_Ac_scalar_plus_eq<qd_real>();
    return s;
}

// Test cases to verify adding Ac via +=.
template <class Scalar>
Status test_specialized_Ac_Ac_plus_eq() {
    Status s;
    { // Verify behavior when adding positive Ac.
        ovd::numeric::Ac<Scalar> a;
        ovd::numeric::Ac<Scalar> b(3);
        a+=b;
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify positive accumulator was used.
        check(3 == a.p[0], s);
    }{ // Verify behavior when adding negative Ac.
        ovd::numeric::Ac<Scalar> a;
        ovd::numeric::Ac<Scalar> b(-3);
        a+=b;
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify negative accumulator was used.
        check(-3 == a.n[0], s);
    }
    return s;
}
Status test_Ac_Ac_plus_eq() {
    Status s;
    s += test_specialized_Ac_Ac_plus_eq<float>();
    s += test_specialized_Ac_Ac_plus_eq<double>();
    s += test_specialized_Ac_Ac_plus_eq<long double>();
    s += test_specialized_Ac_Ac_plus_eq<qd_real>();
    return s;
}


// Test cases to verify subtracting via -=.
template <class Scalar>
Status test_specialized_Ac_scalar_minus_eq() {
    Status s;
    { // Verify behavior when subtracting negative scalar.
        ovd::numeric::Ac<Scalar> a;
        a-=-3;
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify positive accumulator was used.
        check(3 == a.p[0], s);
    }{ // Verify behavior when adding positive scalar.
        ovd::numeric::Ac<Scalar> a;
        a-=3;
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify negative accumulator was used.
        check(-3 == a.n[0], s);
    }
    return s;
}
Status test_Ac_scalar_minus_eq() {
    Status s;
    s += test_specialized_Ac_scalar_minus_eq<float>();
    s += test_specialized_Ac_scalar_minus_eq<double>();
    s += test_specialized_Ac_scalar_minus_eq<long double>();
    s += test_specialized_Ac_scalar_minus_eq<qd_real>();
    return s;
}


// Test cases to verify subtracting Ac via -=.
template <class Scalar>
Status test_specialized_Ac_Ac_minus_eq() {
    Status s;
    { // Verify behavior when subtracting negative Ac.
        ovd::numeric::Ac<Scalar> a;
        ovd::numeric::Ac<Scalar> b(-3);
        a-=b;
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify positive accumulator was used.
        check(3 == a.p[0], s);
    }{ // Verify behavior when subtracting positive Ac.
        ovd::numeric::Ac<Scalar> a;
        ovd::numeric::Ac<Scalar> b(3);
        a-=3;
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify negative accumulator was used.
        check(-3 == a.n[0], s);
    }
    return s;
}
Status test_Ac_Ac_minus_eq() {
    Status s;
    s += test_specialized_Ac_Ac_minus_eq<float>();
    s += test_specialized_Ac_Ac_minus_eq<double>();
    s += test_specialized_Ac_Ac_minus_eq<long double>();
    s += test_specialized_Ac_Ac_minus_eq<qd_real>();
    return s;
}


// A testing-addition policy to verify order of summation.
template <class T>
struct MonitorSummation {
    std::vector<T> plus_eq_operands;
    void plus_eq(T& a, const T& b) {
        plus_eq_operands.push_back(a);
        plus_eq_operands.push_back(b);
        a += b;
    }
};

// Test cases to verify summation.
template <class Scalar>
Status test_specialized_Ac_sum() {
    Status s;
    { // Verify behavior when adding positive scalar.
        ovd::numeric::Ac<Scalar> a;
        a.add(3);
        // Verify sum.
        check(3 == a.sum(), s);
        // Verify accumulators contents are unchanged.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        check(3 == a.p[0], s);
    }{ // Verify behavior when adding negative scalar.
        ovd::numeric::Ac<Scalar> a;
        a.add(-3);
        // Verify sum.
        check(-3 == a.sum(), s);
        // Verify accumulators contents are unchanged.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(-3 == a.n[0], s);
    }{ // Verify behavior when adding positive then negative.
        ovd::numeric::Ac<Scalar> a;
        a.add(3);
        a.add(-3);
        // Verify sum.
        check(0 == a.sum(), s);
        // Verify accumulators contents are unchanged.
        check(1 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(3 == a.p[0], s);
        check(-3 == a.n[0], s);
    }{ // Verify behavior when adding negative then positive.
        ovd::numeric::Ac<Scalar> a;
        a.add(-3);
        a.add(3);
        // Verify sum.
        check(0 == a.sum(), s);
        // Verify accumulators contents are unchanged.
        check(1 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(3 == a.p[0], s);
        check(-3 == a.n[0], s);
    }{ // Verify behavior when adding two positives.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.add(5);
        a.add(3);
        // Verify accumulator sizes and contents.
        check(2 == a.p.size(), s);
        check(0 == a.n.size(), s);
        check(5 == a.p[0], s);
        check(3 == a.p[1], s);
        // Verify sum.
        check(8 == a.sum(), s);
        // Verify order of summation.
        check(4 == a.plus_eq_operands.size(), s);
        check(0 == a.plus_eq_operands[0], s);
        check(3 == a.plus_eq_operands[1], s);
        check(3 == a.plus_eq_operands[2], s);
        check(5 == a.plus_eq_operands[3], s);
        // Verify accumulator sizes are unchanged.
        check(2 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify accumulators contents are sorted.
        check(3 == a.p[0], s);
        check(5 == a.p[1], s);
    }{ // Verify behavior when adding two positives.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.add(-5);
        a.add(-3);
        // Verify accumulator sizes and contents.
        check(0 == a.p.size(), s);
        check(2 == a.n.size(), s);
        check(-5 == a.n[0], s);
        check(-3 == a.n[1], s);
        // Verify sum.
        check(-8 == a.sum(), s);
        // Verify order of summation.
        check(4 == a.plus_eq_operands.size(), s);
        check(0 == a.plus_eq_operands[0], s);
        check(-3 == a.plus_eq_operands[1], s);
        check(-3 == a.plus_eq_operands[2], s);
        check(-5 == a.plus_eq_operands[3], s);
        // Verify accumulator sizes are unchanged.
        check(0 == a.p.size(), s);
        check(2 == a.n.size(), s);
        // Verify accumulators contents are sorted.
        check(-3 == a.n[0], s);
        check(-5 == a.n[1], s);
    }{ // Verify behavior when adding positives and negatives in funky order.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.add(-5);
        a.add(7);
        a.add(3);
        a.add(-13);
        a.add(11);
        a.add(-2);
        // Verify accumulator sizes and contents.
        check(3 == a.p.size(), s);
        check(3 == a.n.size(), s);
        check(7 == a.p[0], s);
        check(3 == a.p[1], s);
        check(11 == a.p[2], s);
        check(-5 == a.n[0], s);
        check(-13 == a.n[1], s);
        check(-2 == a.n[2], s);
        // Verify sum.
        check(1 == a.sum(), s);
        // Verify order of summation.
        check(12 == a.plus_eq_operands.size(), s);
        check(0 == a.plus_eq_operands[0], s);
        check(3 == a.plus_eq_operands[1], s);
        check(3 == a.plus_eq_operands[2], s);
        check(7 == a.plus_eq_operands[3], s);
        check(10 == a.plus_eq_operands[4], s);
        check(11 == a.plus_eq_operands[5], s);
        check(0 == a.plus_eq_operands[6], s);
        check(-2 == a.plus_eq_operands[7], s);
        check(-2 == a.plus_eq_operands[8], s);
        check(-5 == a.plus_eq_operands[9], s);
        check(-7 == a.plus_eq_operands[10], s);
        check(-13 == a.plus_eq_operands[11], s);
        // Verify accumulator sizes are unchanged.
        check(3 == a.p.size(), s);
        check(3 == a.n.size(), s);
        // Verify accumulators contents are sorted.
        check(3 == a.p[0], s);
        check(7 == a.p[1], s);
        check(11 == a.p[2], s);
        check(-2 == a.n[0], s);
        check(-5 == a.n[1], s);
        check(-13 == a.n[2], s);
    }
    return s;
}
Status test_Ac_sum() {
    Status s;
    s += test_specialized_Ac_sum<float>();
    s += test_specialized_Ac_sum<double>();
    s += test_specialized_Ac_sum<long double>();
    s += test_specialized_Ac_sum<qd_real>();
    return s;
}

// Test cases to verify split summation.
template <class Scalar>
Status test_specialized_Ac_split_sum() {
    Status s;
    { // Verify behavior when adding positive scalar.
        ovd::numeric::Ac<Scalar> a;
        a.add(3);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(3 == pos, s);
        check(0 == neg, s);
        // Verify accumulators contents are unchanged.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        check(3 == a.p[0], s);
    }{ // Verify behavior when adding negative scalar.
        ovd::numeric::Ac<Scalar> a;
        a.add(-3);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(0 == pos, s);
        check(-3 == neg, s);
        // Verify accumulators contents are unchanged.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(-3 == a.n[0], s);
    }{ // Verify behavior when adding positive then negative.
        ovd::numeric::Ac<Scalar> a;
        a.add(3);
        a.add(-3);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(3 == pos, s);
        check(-3 == neg, s);
        // Verify accumulators contents are unchanged.
        check(1 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(3 == a.p[0], s);
        check(-3 == a.n[0], s);
    }{ // Verify behavior when adding negative then positive.
        ovd::numeric::Ac<Scalar> a;
        a.add(-3);
        a.add(3);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(3 == pos, s);
        check(-3 == neg, s);
        // Verify accumulators contents are unchanged.
        check(1 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(3 == a.p[0], s);
        check(-3 == a.n[0], s);
    }{ // Verify behavior when adding two positives.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.add(5);
        a.add(3);
        // Verify accumulator sizes and contents.
        check(2 == a.p.size(), s);
        check(0 == a.n.size(), s);
        check(5 == a.p[0], s);
        check(3 == a.p[1], s);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(8 == pos, s);
        check(0 == neg, s);
        // Verify order of summation.
        check(4 == a.plus_eq_operands.size(), s);
        check(0 == a.plus_eq_operands[0], s);
        check(3 == a.plus_eq_operands[1], s);
        check(3 == a.plus_eq_operands[2], s);
        check(5 == a.plus_eq_operands[3], s);
        // Verify accumulator sizes are unchanged.
        check(2 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify accumulators contents are sorted.
        check(3 == a.p[0], s);
        check(5 == a.p[1], s);
    }{ // Verify behavior when adding two positives.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.add(-5);
        a.add(-3);
        // Verify accumulator sizes and contents.
        check(0 == a.p.size(), s);
        check(2 == a.n.size(), s);
        check(-5 == a.n[0], s);
        check(-3 == a.n[1], s);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(0 == pos, s);
        check(-8 == neg, s);
        // Verify order of summation.
        check(4 == a.plus_eq_operands.size(), s);
        check(0 == a.plus_eq_operands[0], s);
        check(-3 == a.plus_eq_operands[1], s);
        check(-3 == a.plus_eq_operands[2], s);
        check(-5 == a.plus_eq_operands[3], s);
        // Verify accumulator sizes are unchanged.
        check(0 == a.p.size(), s);
        check(2 == a.n.size(), s);
        // Verify accumulators contents are sorted.
        check(-3 == a.n[0], s);
        check(-5 == a.n[1], s);
    }{ // Verify behavior when adding positives and negatives in funky order.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.add(-5);
        a.add(7);
        a.add(3);
        a.add(-13);
        a.add(11);
        a.add(-2);
        // Verify accumulator sizes and contents.
        check(3 == a.p.size(), s);
        check(3 == a.n.size(), s);
        check(7 == a.p[0], s);
        check(3 == a.p[1], s);
        check(11 == a.p[2], s);
        check(-5 == a.n[0], s);
        check(-13 == a.n[1], s);
        check(-2 == a.n[2], s);
        // Verify split sum.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(21 == pos, s);
        check(-20 == neg, s);
        // Verify order of summation.
        check(12 == a.plus_eq_operands.size(), s);
        check(0 == a.plus_eq_operands[0], s);
        check(3 == a.plus_eq_operands[1], s);
        check(3 == a.plus_eq_operands[2], s);
        check(7 == a.plus_eq_operands[3], s);
        check(10 == a.plus_eq_operands[4], s);
        check(11 == a.plus_eq_operands[5], s);
        check(0 == a.plus_eq_operands[6], s);
        check(-2 == a.plus_eq_operands[7], s);
        check(-2 == a.plus_eq_operands[8], s);
        check(-5 == a.plus_eq_operands[9], s);
        check(-7 == a.plus_eq_operands[10], s);
        check(-13 == a.plus_eq_operands[11], s);
        // Verify accumulator sizes are unchanged.
        check(3 == a.p.size(), s);
        check(3 == a.n.size(), s);
        // Verify accumulators contents are sorted.
        check(3 == a.p[0], s);
        check(7 == a.p[1], s);
        check(11 == a.p[2], s);
        check(-2 == a.n[0], s);
        check(-5 == a.n[1], s);
        check(-13 == a.n[2], s);
    }
    return s;
}
Status test_Ac_split_sum() {
    Status s;
    s += test_specialized_Ac_split_sum<float>();
    s += test_specialized_Ac_split_sum<double>();
    s += test_specialized_Ac_split_sum<long double>();
    s += test_specialized_Ac_split_sum<qd_real>();
    return s;
}


// Test cases to verify constructors.
template <class Scalar>
Status test_specialized_Ac_constructors() {
    Status s;
    { // Verify copy constructor.
        ovd::numeric::Ac<Scalar> a;
        a.p.push_back(11);
        a.p.push_back(2);
        a.p.push_back(3);
        a.n.push_back(-7);
        a.n.push_back(-13);
        a.n.push_back(-5);
        ovd::numeric::Ac<Scalar> b(a);
        // Verify accumulator sizes.
        check(3 == b.p.size(), s);
        check(3 == b.n.size(), s);
        // Verify accumulator contents.
        check(11 == b.p[0], s);
        check(2 == b.p[1], s);
        check(3 == b.p[2], s);
        check(-7 == b.n[0], s);
        check(-13 == b.n[1], s);
        check(-5 == b.n[2], s);
    }{ // Verify empty constructor.
        ovd::numeric::Ac<Scalar> a;
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(0 == a.n.size(), s);
    }{ // Verify constructor with one positive scalar.
        ovd::numeric::Ac<Scalar> a(1);
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify correct accumulator was used.
        check(Scalar(1) == a.p[0], s);
    }{ // Verify constructor with one negative scalar.
        ovd::numeric::Ac<Scalar> a(-1);
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify correct accumulator was used.
        check(Scalar(-1) == a.n[0], s);
    }{ // Verify constructor with one negative and one positive scalar.
        ovd::numeric::Ac<Scalar> a(1,-2);
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify correct accumulators were used.
        check(Scalar(1) == a.p[0], s);
        check(Scalar(-2) == a.n[0], s);
    }{
        ovd::numeric::Ac<Scalar> a(-1,2);
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(1 == a.n.size(), s);
        // Verify correct accumulators were used.
        check(Scalar(2) == a.p[0], s);
        check(Scalar(-1) == a.n[0], s);
    }{ // Verify three-scalar constructor.
        ovd::numeric::Ac<Scalar> a(-1,2,-3);
        // Verify accumulator sizes.
        check(1 == a.p.size(), s);
        check(2 == a.n.size(), s);
        // Verify accumulators were used in correct order.
        check(Scalar(2) == a.p[0], s);
        check(Scalar(-1) == a.n[0], s);
        check(Scalar(-3) == a.n[1], s);
    }{ // Verify four-scalar constructor.
        ovd::numeric::Ac<Scalar> a(-1,2,-3,5);
        // Verify accumulator sizes.
        check(2 == a.p.size(), s);
        check(2 == a.n.size(), s);
        // Verify accumulators were used in correct order.
        check(Scalar(2) == a.p[0], s);
        check(Scalar(5) == a.p[1], s);
        check(Scalar(-1) == a.n[0], s);
        check(Scalar(-3) == a.n[1], s);
    }{ // Verify five-scalar constructor.
        ovd::numeric::Ac<Scalar> a(-1,2,-3,5,-7);
        // Verify accumulator sizes.
        check(2 == a.p.size(), s);
        check(3 == a.n.size(), s);
        // Verify accumulators were used in correct order.
        check(Scalar(2) == a.p[0], s);
        check(Scalar(5) == a.p[1], s);
        check(Scalar(-1) == a.n[0], s);
        check(Scalar(-3) == a.n[1], s);
        check(Scalar(-7) == a.n[2], s);
    }{ // Verify six-scalar constructor.
        ovd::numeric::Ac<Scalar> a(-1,2,-3,5,-7,11);
        // Verify accumulator sizes.
        check(3 == a.p.size(), s);
        check(3 == a.n.size(), s);
        // Verify accumulators were used in correct order.
        check(Scalar(2) == a.p[0], s);
        check(Scalar(5) == a.p[1], s);
        check(Scalar(11) == a.p[2], s);
        check(Scalar(-1) == a.n[0], s);
        check(Scalar(-3) == a.n[1], s);
        check(Scalar(-7) == a.n[2], s);
    }
    return s;
}
Status test_Ac_constructors() {
    Status s;
    s += test_specialized_Ac_constructors<float>();
    s += test_specialized_Ac_constructors<double>();
    s += test_specialized_Ac_constructors<long double>();
    s += test_specialized_Ac_constructors<qd_real>();
    return s;
}


// Test cases to verify Ac assignment operator.
template <class Scalar>
Status test_specialized_Ac_Ac_assignment() {
    Status s;
    { // Verify Ac assignment operator.
        ovd::numeric::Ac<Scalar> a;
        a.p.push_back(11);
        a.p.push_back(2);
        a.p.push_back(3);
        a.n.push_back(-7);
        a.n.push_back(-13);
        a.n.push_back(-5);
        ovd::numeric::Ac<Scalar> b = a;
        // Verify accumulator sizes.
        check(3 == b.p.size(), s);
        check(3 == b.n.size(), s);
        // Verify accumulator contents.
        check(11 == b.p[0], s);
        check(2 == b.p[1], s);
        check(3 == b.p[2], s);
        check(-7 == b.n[0], s);
        check(-13 == b.n[1], s);
        check(-5 == b.n[2], s);
    }
    return s;
}
Status test_Ac_Ac_assignment() {
    Status s;
    s += test_specialized_Ac_Ac_assignment<float>();
    s += test_specialized_Ac_Ac_assignment<double>();
    s += test_specialized_Ac_Ac_assignment<long double>();
    s += test_specialized_Ac_Ac_assignment<qd_real>();
    return s;
}


// Test cases to verify scalar assignment operator.
template <class Scalar>
Status test_specialized_Ac_scalar_assignment() {
    Status s;
    { // Verify assignment of positive scalar.
        ovd::numeric::Ac<Scalar> b = Scalar(5);
        // Verify accumulator sizes.
        check(1 == b.p.size(), s);
        check(0 == b.n.size(), s);
        // Verify accumulator contents.
        check(5 == b.p[0], s);
    }
    { // Verify assignment of negative scalar.
        ovd::numeric::Ac<Scalar> b = Scalar(-5);
        // Verify accumulator sizes.
        check(0 == b.p.size(), s);
        check(1 == b.n.size(), s);
        // Verify accumulator contents.
        check(-5 == b.n[0], s);
    }
    return s;
}
Status test_Ac_scalar_assignment() {
    Status s;
    s += test_specialized_Ac_scalar_assignment<float>();
    s += test_specialized_Ac_scalar_assignment<double>();
    s += test_specialized_Ac_scalar_assignment<long double>();
    s += test_specialized_Ac_scalar_assignment<qd_real>();
    return s;
}


// Test cases to verify clear().
template <class Scalar>
Status test_specialized_Ac_clear() {
    Status s;
    { // Verify clear() method.
        ovd::numeric::Ac<Scalar> a;
        a.p.push_back(11);
        a.p.push_back(2);
        a.p.push_back(3);
        a.n.push_back(-7);
        a.n.push_back(-13);
        a.n.push_back(-5);
        a.clear();
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify summation.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(0 == pos, s);
        check(0 == pos, s);
    }
    { // Verify no summation operations after clear() method.
        ovd::numeric::Ac<Scalar, MonitorSummation> a;
        a.p.push_back(11);
        a.p.push_back(2);
        a.p.push_back(3);
        a.n.push_back(-7);
        a.n.push_back(-13);
        a.n.push_back(-5);
        a.clear();
        // Verify accumulator sizes.
        check(0 == a.p.size(), s);
        check(0 == a.n.size(), s);
        // Verify no operations upon summation.
        a.sum();
        check(0 == a.plus_eq_operands.size(), s);
        // Verify no operations upon split summation.
        Scalar pos, neg;
        a.sum(pos, neg);
        check(0 == a.plus_eq_operands.size(), s);
    }
    return s;
}
Status test_Ac_clear() {
    Status s;
    s += test_specialized_Ac_clear<float>();
    s += test_specialized_Ac_clear<double>();
    s += test_specialized_Ac_clear<long double>();
    s += test_specialized_Ac_clear<qd_real>();
    return s;
}


// Test cases to verify product().
template <class Scalar>
Status test_specialized_product() {
    Status s;
    { // Verify product() function.
        vector<Scalar> a;
        a.push_back(11);
        a.push_back(-7);
        a.push_back(2);
        vector<Scalar> b;
        b.push_back(3);
        b.push_back(-13);
        b.push_back(-5);
        ovd::numeric::Ac<Scalar> result;
        product(a, b, result);

        // Verify result accumulator sizes.
        check(4 == result.p.size(), s);
        check(5 == result.n.size(), s);
        // Verify result accumulator contents.
        check(11*3 == result.p[0], s);
        check(11*(-13) == result.n[0], s);
        check(11*(-5) == result.n[1], s);
        check((-7)*3 == result.n[2], s);
        check((-7)*(-13) == result.p[1], s);
        check((-7)*(-5) == result.p[2], s);
        check(2*3 == result.p[3], s);
        check(2*(-13) == result.n[3], s);
        check(2*(-5) == result.n[4], s);

        // Verify "a" is unaltered.
        check(3 == a.size(), s);
        check(11 == a[0], s);
        check(-7 == a[1], s);
        check(2 == a[2], s);

        // Verify "b" is unaltered.
        check(3 == b.size(), s);
        check(3 == b[0], s);
        check(-13 == b[1], s);
        check(-5 == b[2], s);
    }
    return s;
}
Status test_product() {
    Status s;
    s += test_specialized_product<float>();
    s += test_specialized_product<double>();
    s += test_specialized_product<long double>();
    s += test_specialized_product<qd_real>();
    return s;
}


// Test cases to verify operator*(Ac<T>&, T).
template <class Scalar>
Status test_specialized_operator_star_Ac_T() {
    Status s;
    {
        ovd::numeric::Ac<Scalar> a;
        a.add(11);
        a.add(-7);
        a.add(2);
        ovd::numeric::Ac<Scalar> result = a*Scalar(3);
        // Verify result accumulator sizes.
        check(2 == result.p.size(), s);
        check(1 == result.n.size(), s);
        // Verify result accumulator contents.
        check(11*3 == result.p[0], s);
        check((-7)*3 == result.n[0], s);
        check(2*3 == result.p[1], s);
        // Verify "a" is unaltered.
        check(2 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(11 == a.p[0], s);
        check(-7 == a.n[0], s);
        check(2 == a.p[1], s);
    }
    return s;
}
Status test_operator_star_Ac_T() {
    Status s;
    s += test_specialized_operator_star_Ac_T<float>();
    s += test_specialized_operator_star_Ac_T<double>();
    s += test_specialized_operator_star_Ac_T<long double>();
    s += test_specialized_operator_star_Ac_T<qd_real>();
    return s;
}


// Test cases to verify operator*(T, Ac<T>&).
template <class Scalar>
Status test_specialized_operator_star_T_Ac() {
    Status s;
    {
        ovd::numeric::Ac<Scalar> a;
        a.add(11);
        a.add(-7);
        a.add(2);
        ovd::numeric::Ac<Scalar> result = Scalar(3)*a;
        // Verify result accumulator sizes.
        check(2 == result.p.size(), s);
        check(1 == result.n.size(), s);
        // Verify result accumulator contents.
        check(11*3 == result.p[0], s);
        check((-7)*3 == result.n[0], s);
        check(2*3 == result.p[1], s);
        // Verify "a" is unaltered.
        check(2 == a.p.size(), s);
        check(1 == a.n.size(), s);
        check(11 == a.p[0], s);
        check(-7 == a.n[0], s);
        check(2 == a.p[1], s);
    }
    return s;
}
Status test_operator_star_T_Ac() {
    Status s;
    s += test_specialized_operator_star_T_Ac<float>();
    s += test_specialized_operator_star_T_Ac<double>();
    s += test_specialized_operator_star_T_Ac<long double>();
    s += test_specialized_operator_star_T_Ac<qd_real>();
    return s;
}


// Brainstorming...
template <class Scalar>
Status limits() {
    Status s;
    numeric_limits<Scalar> x;
    cout << "x.min(): " << x.min() << endl;
    cout << "x.max(): " << x.max() << endl;
    cout << "x.digits: " << x.digits << endl;
    cout << "x.digits10: " << x.digits10 << endl;
    cout << "x.is_signed: " << x.is_signed << endl;
    cout << "x.is_integer: " << x.is_integer << endl;
    cout << "x.is_exact: " << x.is_exact << endl;
    cout << "x.radix: " << x.radix << endl;
    cout << "x.epsilon(): " << x.epsilon() << endl;
    cout << "x.round_error(): " << x.round_error() << endl;
    cout << "x.min_exponent: " << x.min_exponent << endl;
    cout << "x.min_exponent10: " << x.min_exponent10 << endl;
    cout << "x.max_exponent: " << x.max_exponent << endl;
    cout << "x.max_exponent10: " << x.max_exponent10 << endl;
    cout << "x.has_infinity: " << x.has_infinity << endl;
    cout << "x.has_quiet_NaN: " << x.has_quiet_NaN << endl;
    cout << "x.has_signaling_NaN: " << x.has_signaling_NaN << endl;
    cout << "x.has_denorm: ";
    switch (x.has_denorm) {
      case denorm_indeterminate: cout << "denorm_indeterminate"; break;
      case denorm_absent: cout << "denorm_absent"; break;
      case denorm_present: cout << "denorm_present"; break;
      default: cout << "unknown"; break;
    }
    cout << endl;
    cout << "x.has_denorm_loss: " << x.has_denorm_loss << endl;
    cout << "x.infinity(): " << x.infinity() << endl;
    cout << "x.quiet_NaN(): " << x.quiet_NaN() << endl;
    cout << "x.signaling_NaN(): " << x.signaling_NaN() << endl;
    cout << "x.denorm_min(): " << x.denorm_min() << endl;
    cout << "x.is_iec559: " << x.is_iec559 << endl;
    cout << "x.is_bounded: " << x.is_bounded << endl;
    cout << "x.is_modulo: " << x.is_modulo << endl;
    cout << "x.traps: " << x.traps << endl;
    cout << "x.tinyness_before: " << x.tinyness_before << endl;
    cout << "x.round_style: ";
    switch (x.round_style) {
      case round_toward_zero: cout << "round_toward_zero"; break;
      case round_to_nearest: cout << "round_to_nearest"; break;
      case round_toward_infinity: cout << "round_toward_infinity"; break;
      case round_toward_neg_infinity: cout << "round_toward_neg_infinity"; break;
      case round_indeterminate: cout << "round_indeterminate"; break;
      default: cout << "unknown"; break;
    }
    cout << endl;
    //Scalar c0+e = a+b = c;
    // e = c-(a+b)
    Scalar a(2.), b(x.epsilon()), c0(a+b);
    Scalar e;
    e = c0;
    e -= a;
    e -= b;
    cout << "e: " << e << endl;

    ovd::numeric::Ac<Scalar> c(a, b);
    c -= a;
    c -= b;
    return s;
}
Status brainstorm() {
    Status s;
    cout << "float:" << endl;
    limits<float>();
    cout << endl;
    return s;
}


int main(int argc, char **argv) {
    Status s;
    std::cout << ovd::version() << "\n"; // the git revision-string
    s += test_Ac_add();
    s += test_Ac_scalar_plus_eq();
    s += test_Ac_Ac_plus_eq();
    s += test_Ac_scalar_minus_eq();
    s += test_Ac_Ac_minus_eq();
    s += test_Ac_sum();
    s += test_Ac_split_sum();
    s += test_Ac_constructors();
    s += test_Ac_Ac_assignment();
    s += test_Ac_scalar_assignment();
    s += test_Ac_clear();
    s += test_product();
    s += test_operator_star_Ac_T();
    s += test_operator_star_T_Ac();
    s += brainstorm();
    // Say how many checks were performed, and how many failed.
    cout << s.cases << " checks, " << s.errors << " errors." << endl;
    return bool(s.errors);
}
