#ifndef GLOBAL_H
#define GLOBAL_H

/* Simple #defines copy/pasted from http://rlove.org/log/2005102601 */
#if __GNUC__ >= 3
# define inline		inline __attribute__ ((always_inline))
# define __pure		__attribute__ ((pure))
# define __const	__attribute__ ((const))
# define __noreturn	__attribute__ ((noreturn))
# define __malloc	__attribute__ ((malloc))
# define __must_check	__attribute__ ((warn_unused_result))
# define __deprecated	__attribute__ ((deprecated))
# define __used		__attribute__ ((used))
# define __unused	__attribute__ ((unused))
# define __packed	__attribute__ ((packed))
# define likely(x)	__builtin_expect (!!(x), 1)
# define unlikely(x)	__builtin_expect (!!(x), 0)
#else
# define inline		/* no inline */
# define __pure		/* no pure */
# define __const	/* no const */
# define __noreturn	/* no noreturn */
# define __malloc	/* no malloc */
# define __must_check	/* no warn_unused_result */
# define __deprecated	/* no deprecated */
# define __used		/* no used */
# define __unused	/* no unused */
# define __packed	/* no packed */
# define likely(x)	(x)
# define unlikely(x)	(x)
#endif

/* Useful templates */
template<typename T>
static inline __const T max(T i1, T i2) { return i1 > i2 ? i1 : i2; }
template<typename T>
static inline __const T min(T i1, T i2) { return i1 < i2 ? i1 : i2; }

#endif /* GLOBAL_H */
