#ifndef NT_HPP_
#define NT_HPP_

#include <vector>
#include <numeric>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <inverse.hpp>

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
  assert(n > 0);
  std::vector<T> res;
  T i = 1;
  for (; (unsigned long long) i * i < (unsigned long long) n; ++i) {
    if (n % i == 0) {
      res.push_back(i);
    }
  }
  int j = (int) res.size() - 1;
  if ((unsigned long long) i * i == (unsigned long long) n)
    res.push_back(i);
  while (j >= 0) {
    res.push_back(n / res[j]);
    --j;
  }
  return res;
}

template<typename T> std::vector<std::pair<T, int>> factorize_p(T n) {
  assert(n > 0);
  std::vector<std::pair<T, int>> res;
  unsigned long long N = n;
  for (unsigned long long i = 2; i * i <= N; ++i) {
    int cnt = 0;
    while (N % i == 0) {
      N /= i;
      ++cnt;
    }
    if (cnt) {
      res.emplace_back(i, cnt);
    }
  }
  if (N > 1) {
    res.emplace_back(N, 1);
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

long long gcd(long long a, long long b, long long& x, long long& y) {
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

template<typename T> T phi(T n) {
  assert(n > 0);
  T result = 1;
  for (T i = 2; i * i <= n; ++i) {
    if (n % i == 0) {
      n /= i;
      T t = 1;
      while (n % i == 0) {
        n /= i;
        t *= i;
      }
      result *= t * (i - 1);
    }
  }
  if (n > 1)
    result *= n - 1;
  return result;
}

/// @brief Compute Mobius function for positive integers not greater than n.
/// @return A list \c mu of integers, \c mu[0] is 0.
// https://mathoverflow.net/a/99545/303233
std::vector<int> get_mu(int n) {
  assert(n > 0);
  std::vector<int> mu(n + 1, 1);
  mu[0] = 0;
  for (int i = 2; i * i <= n; i++) {
    if (mu[i] == 1) {// i is a prime number.
      for (int j = i; j <= n; j += i) {
        mu[j] *= -i;
      }
      for (int k = i * i, j = k; j <= n; j += k) {
        mu[j] = 0;
      }
    }
  }
  for (int i = 2; i <= n; i++) {
    if (mu[i] == i)
      mu[i] = 1;
    else if (mu[i] == -i)
      mu[i] = -1;
    else if (mu[i] > 0)
      mu[i] = -1;
    else if (mu[i] < 0)
      mu[i] = 1;
    // else mu[i] == 0
  }
  return mu;
}

struct mpf_info {
  int mpf = 0;
  int mpf_cnt;
  int next;
};

std::vector<mpf_info> get_mpf_info(int n) {
  assert(n > 0);
  std::vector<int> primes;
  std::vector<mpf_info> res(n + 1);
  for (int i = 2; i <= n; i++) {
    if (res[i].mpf == 0) {
      res[i] = {i, 1, 1};
      primes.push_back(i);
    }
    int lim = n / i;
    int max_prime = std::min(lim, res[i].mpf - 1);
    for (int p : primes) {
      if (p > max_prime)
        break;
      res[p * i] = {p, 1, i};
    }
    if (res[i].mpf <= lim)
      res[res[i].mpf * i] = {res[i].mpf, res[i].mpf_cnt + 1, res[i].next};
  }
  return res;
}

int mod_pow(long long a, long long n, int mod) {
  assert(n >= 0);
  assert(mod > 0);
  a %= mod;
  if (a < 0)
    a+= mod;
  long long ans = 1;
  while (n > 0) {
    if (n & 1)
      ans = a * ans % mod;
    a = a * a % mod;
    n >>= 1;
  }
  return (int) ans;
}

int discrete_log(int a, int b, int mod) {
  assert(mod > 1);
  assert(std::gcd(a, mod) == 1);

  if (std::gcd(b, mod) != 1)
    return -1;

  a %= mod;
  b %= mod;
  if (a < 0)
    a += mod;
  if (b < 0)
    b += mod;

  int n = phi(mod);
  int m = std::sqrt(n);
  if (m * m < n)
    m++;
  std::unordered_map<int, int> J;
  long long t = 1;
  for (int j = 0; j < m; j++) {
    J.insert({(int) t, j});
    t = t * a % mod;
  }
  int k = mod_pow(inverse(a, mod), m, mod);
  t = b;
  for (int i = 0; i < m; i++) {
    auto it = J.find((int) t);
    if (it != J.end())
      return i * m + it->second;
    t = t * k % mod;
  }
  return -1;
}

int discrete_log_mod_p(int a, int b, int p) {
  assert(p > 1);
  assert(std::gcd(a, p) == 1);

  if (std::gcd(b, p) != 1)
    return -1;

  a %= p;
  b %= p;
  if (a < 0)
    a += p;
  if (b < 0)
    b += p;
  int m = std::sqrt(p - 1);
  if (m * m < p - 1)
    m++;
  std::unordered_map<int, int> J;
  long long t = 1;
  for (int j = 0; j < m; j++) {
    J.insert({(int) t, j});
    t = t * a % p;
  }
  int k = mod_pow(inverse(a, p), m, p);
  t = b;
  for (int i = 0; i < m; i++) {
    auto it = J.find((int) t);
    if (it != J.end())
      return i * m + it->second;
    t = t * k % p;
  }
  return -1;
}

#endif// NT_HPP_
