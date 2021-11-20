#include <vector>

template<typename T> std::vector<T> prime_divisors(T n) {
  std::vector<T> res;
  for (T i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      res.push_back(i);
      do {
        n /= i;
      } while (n % i == 0);
    }
  }
  if (n > 1)
    res.push_back(n);
  return res;
}

template<typename T> std::vector<T> divisors(T n) {
  std::vector<T> res;
  T i = 1;
  for (; (unsigned long long) i * i < n; ++i) {
    if (n % i == 0) {
      res.push_back(i);
    }
  }
  int j = (int) res.size() - 1;
  if ((unsigned long long) i * i == n)
    res.push_back(i);
  while (j >= 0) {
    res.push_back(n / res[j]);
    --j;
  }
  return res;
}

template<typename T> std::vector<std::pair<T, int>> factorize_p(T n) {
  std::vector<std::pair<T, int>> res;
  for (T i = 2; (unsigned long long) i * i <= n; ++i) {
    int cnt = 0;
    while (n % i == 0) {
      n /= i;
      ++cnt;
    }
    if (cnt) {
      res.emplace_back(i, cnt);
    }
  }
  if (n > 1) {
    res.emplace_back(n, 1);
  }
  return res;
}

template<typename T> std::vector<T> factorize(T n) {
  std::vector<T> res;
  for (T i = 2; (unsigned long long) i * i <= n; ++i) {
    T t = 1;
    while (n % i == 0) {
      t *= i;
      n /= i;
    }
    if (t > 1)
      res.push_back(t);
  }
  if (n > 1)
    res.push_back(n);
  return res;
}

/// @return a list of primes not greater than \p n.
std::vector<int> get_primes(int n) {
  std::vector<int> mpf(n + 1);
  std::vector<int> primes;
  for (int i = 2; i <= n; i++) {
    if (mpf[i] == 0) {
      mpf[i] = i;
      primes.push_back(i);
    }
    const int max_p = std::min(mpf[i], n / i);
    for (auto p : primes) {
      if (p <= max_p)
        mpf[p * i] = p;
      else
        break;
    }
  }
  return primes;
}

std::vector<int> get_mpf(int n) {
  std::vector<int> mpf(n + 1);
  std::vector<int> primes;
  for (int i = 2; i <= n; i++) {
    if (mpf[i] == 0) {
      mpf[i] = i;
      primes.push_back(i);
    }
    const int max_p = std::min(mpf[i], n / i);
    for (int p : primes) {
      if (p <= max_p)
        mpf[p * i] = p;
      else
        break;
    }
  }
  return mpf;
}

long long gcd(long long a, long long b, long long &x, long long &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  long long t = gcd(b, a % b, y, x);
  // y * b + x * (a % b) == gcd(a, b)
  // y * b + x * (a - (a / b) * b) == gcd(a, b)
  // (y - x * (a / b)) * b + x * a == gcd(a, b)
  y -= x * (a / b);
  return t;
}
