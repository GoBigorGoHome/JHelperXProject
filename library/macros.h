//
// Created by zjsdu on 2/9/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MACROS_H_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MACROS_H_

#define CALL_WITH_EXPANDED_ARGS(function, ...) function(__VA_ARGS__)

#define JOIN_IMPL(arg1, arg2) arg1##arg2
#define JOIN(arg1, arg2) JOIN_IMPL(arg1, arg2)

#define EXPAND_1(...) __VA_ARGS__
#define EXPAND_4(...) EXPAND_1(EXPAND_1(EXPAND_1(__VA_ARGS__)))
#define EXPAND_13(...) EXPAND_4(EXPAND_4(EXPAND_4(__VA_ARGS__)))

#define PAUSE
#define COMMA() ,
#define TERMINATE(...)
#define SELECT_SECOND_ARG(arg1, arg2, ...) arg2
#define CONDITIONAL(peek, arg1, arg2)                                          \
  CALL_WITH_EXPANDED_ARGS(SELECT_SECOND_ARG, COMMA peek arg1, arg2)
#define TERMINATE_OR(peek, arg) CONDITIONAL(peek, TERMINATE, arg)

#define FOR_EACH_2_IMPL0(function, arg1, arg2, peek, ...)                      \
  function(arg1, arg2) TERMINATE_OR(peek, FOR_EACH_2_IMPL1)                    \
      PAUSE(function, peek, __VA_ARGS__)

#define FOR_EACH_2_IMPL1(function, arg1, arg2, peek...)                        \
  function(arg1, arg2) TERMINATE_OR(peek, FOR_EACH_2_IMPL0)                    \
      PAUSE(function, peek, __VA_ARGS__)

#define FOR_EACH_2(function, ...)                                              \
  EXPAND_13(FOR_EACH_2_IMPL0(function, __VA_ARGS__, ()))

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MACROS_H_
