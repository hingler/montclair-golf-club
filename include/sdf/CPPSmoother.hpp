#ifndef CPP_SMOOTHER_H_
#define CPP_SMOOTHER_H_

#include <algorithm>
#include <cstdlib>
#include <memory>

// negating?

namespace mgc {
  template <typename SDF_A, typename SDF_B>
  class CPPSmoother {
   public:
    CPPSmoother(
      const std::shared_ptr<SDF_A>& a,
      double fac_a,
      const std::shared_ptr<SDF_B>& b,
      double fac_b,
      double k,
      double fac_final
    ) : a(a), fac_a(fac_a), b(b), fac_b(fac_b), k(k), fac_final(fac_final) {}


    double Sample(double x, double y) const {
      double dist_a = a->Sample(x, y) * fac_a;
      double dist_b = b->Sample(x, y) * fac_b;

      return smin_f(
        dist_a, dist_b, k
      ) * fac_final;
    }

    double Sample(double x, double y, double z) const {
      double dist_a = a->Sample(x, y, z) * fac_a;
      double dist_b = b->Sample(x, y, z) * fac_b;

      return smin_f(
        dist_a, dist_b, k
      ) * fac_final;
    }

   private:
    std::shared_ptr<SDF_A> a;
    double fac_a;
    std::shared_ptr<SDF_B> b;
    double fac_b;
    double k;

    double fac_final;

    static constexpr double K_EPSILON = 0.0001;

    double smin_f(double a, double b, double k) const {
      if (k < K_EPSILON) {
        // no smoothing
        return std::min(a, b);
      } else {
        // k is 0 - div0 error
        double h = std::max(k - std::abs(a - b), 0.0) / k;
        return std::min(a, b) - h * h * k * 0.25;
      }
    }
  };

  namespace smooth {
    template <typename SDF_A, typename SDF_B>
    CPPSmoother<SDF_A, SDF_B> SmootherSMin(
      const std::shared_ptr<SDF_A>& a,
      const std::shared_ptr<SDF_B>& b,
      double k
    ) {
      return CPPSmoother<SDF_A, SDF_B>(
        a,
        1.0,
        b,
        1.0,
        k,
        1.0
      );
    }

    template <typename SDF_A, typename SDF_B>
    CPPSmoother<SDF_B, SDF_A> SmootherSSub(
      const std::shared_ptr<SDF_A>& a,
      const std::shared_ptr<SDF_B>& b,
      double k
    ) {
      return CPPSmoother<SDF_B, SDF_A>(
        b,
        1.0,
        a,
        -1.0,
        k,
        -1.0
      );
    }
  }
}

#endif // CPP_SMOOTHER_H_
