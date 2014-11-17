#include "gtest-strip.h"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#ifndef GTEST_INCLUDE_GTEST_GTEST_SPI_H_
#define GTEST_INCLUDE_GTEST_GTEST_SPI_H_ 
namespace testing {
class GTEST_API_ ScopedFakeTestPartResultReporter
    : public TestPartResultReporterInterface {
 public:
  enum InterceptMode {
    INTERCEPT_ONLY_CURRENT_THREAD,
    INTERCEPT_ALL_THREADS
  };
  explicit ScopedFakeTestPartResultReporter(TestPartResultArray* result);
  ScopedFakeTestPartResultReporter(InterceptMode intercept_mode,
                                   TestPartResultArray* result);
  virtual ~ScopedFakeTestPartResultReporter();
  virtual void ReportTestPartResult(const TestPartResult& result);
 private:
  void Init();
  const InterceptMode intercept_mode_;
  TestPartResultReporterInterface* old_reporter_;
  TestPartResultArray* const result_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(ScopedFakeTestPartResultReporter);
};
namespace internal {
class GTEST_API_ SingleFailureChecker {
 public:
  SingleFailureChecker(const TestPartResultArray* results,
                       TestPartResult::Type type,
                       const string& substr);
  ~SingleFailureChecker();
 private:
  const TestPartResultArray* const results_;
  const TestPartResult::Type type_;
  const string substr_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(SingleFailureChecker);
};
}
}
#define EXPECT_FATAL_FAILURE(statement,substr) \
  do { \
    class GTestExpectFatalFailureHelper {\
     public:\
      static void Execute() { statement; }\
    };\
    ::testing::TestPartResultArray gtest_failures;\
    ::testing::internal::SingleFailureChecker gtest_checker(\
        &gtest_failures, ::testing::TestPartResult::kFatalFailure, (substr));\
    {\
      ::testing::ScopedFakeTestPartResultReporter gtest_reporter(\
          ::testing::ScopedFakeTestPartResultReporter:: \
          INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);\
      GTestExpectFatalFailureHelper::Execute();\
    }\
  } while (::testing::internal::AlwaysFalse())
#define EXPECT_FATAL_FAILURE_ON_ALL_THREADS(statement,substr) \
  do { \
    class GTestExpectFatalFailureHelper {\
     public:\
      static void Execute() { statement; }\
    };\
    ::testing::TestPartResultArray gtest_failures;\
    ::testing::internal::SingleFailureChecker gtest_checker(\
        &gtest_failures, ::testing::TestPartResult::kFatalFailure, (substr));\
    {\
      ::testing::ScopedFakeTestPartResultReporter gtest_reporter(\
          ::testing::ScopedFakeTestPartResultReporter:: \
          INTERCEPT_ALL_THREADS, &gtest_failures);\
      GTestExpectFatalFailureHelper::Execute();\
    }\
  } while (::testing::internal::AlwaysFalse())
#define EXPECT_NONFATAL_FAILURE(statement,substr) \
  do {\
    ::testing::TestPartResultArray gtest_failures;\
    ::testing::internal::SingleFailureChecker gtest_checker(\
        &gtest_failures, ::testing::TestPartResult::kNonFatalFailure, \
        (substr));\
    {\
      ::testing::ScopedFakeTestPartResultReporter gtest_reporter(\
          ::testing::ScopedFakeTestPartResultReporter:: \
          INTERCEPT_ONLY_CURRENT_THREAD, &gtest_failures);\
      if (::testing::internal::AlwaysTrue()) { statement; }\
    }\
  } while (::testing::internal::AlwaysFalse())
#define EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS(statement,substr) \
  do {\
    ::testing::TestPartResultArray gtest_failures;\
    ::testing::internal::SingleFailureChecker gtest_checker(\
        &gtest_failures, ::testing::TestPartResult::kNonFatalFailure, \
        (substr));\
    {\
      ::testing::ScopedFakeTestPartResultReporter gtest_reporter(\
          ::testing::ScopedFakeTestPartResultReporter::INTERCEPT_ALL_THREADS, \
          &gtest_failures);\
      if (::testing::internal::AlwaysTrue()) { statement; }\
    }\
  } while (::testing::internal::AlwaysFalse())
#endif
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include <vector>
#if GTEST_OS_LINUX
#define GTEST_HAS_GETTIMEOFDAY_ 1
# include <fcntl.h>
# include <limits.h>
# include <sched.h>
# include <strings.h>
# include <sys/mman.h>
# include <sys/time.h>
# include <unistd.h>
# include <string>
#elif GTEST_OS_SYMBIAN
#define GTEST_HAS_GETTIMEOFDAY_ 1
# include <sys/time.h>
#elif GTEST_OS_ZOS
#define GTEST_HAS_GETTIMEOFDAY_ 1
# include <sys/time.h>
# include <strings.h>
#elif GTEST_OS_WINDOWS_MOBILE
# include <windows.h>
#elif GTEST_OS_WINDOWS
# include <io.h>
# include <sys/timeb.h>
# include <sys/types.h>
# include <sys/stat.h>
# if GTEST_OS_WINDOWS_MINGW
#define GTEST_HAS_GETTIMEOFDAY_ 1
# include <sys/time.h>
# endif
# include <windows.h>
#else
#define GTEST_HAS_GETTIMEOFDAY_ 1
# include <sys/time.h>
# include <unistd.h>
#endif
#if GTEST_HAS_EXCEPTIONS
# include <stdexcept>
#endif
#if GTEST_CAN_STREAM_RESULTS_
# include <arpa/inet.h>
# include <netdb.h>
#endif
#define GTEST_IMPLEMENTATION_ 1
#ifndef GTEST_SRC_GTEST_INTERNAL_INL_H_
#define GTEST_SRC_GTEST_INTERNAL_INL_H_ 
#if !GTEST_IMPLEMENTATION_
# error "gtest-internal-inl.h is part of Google Test's internal implementation."
# error "It must not be included except by Google Test itself."
#endif
#ifndef _WIN32_WCE
# include <errno.h>
#endif
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <vector>
#if GTEST_CAN_STREAM_RESULTS_
# include <arpa/inet.h>
# include <netdb.h>
#endif
#if GTEST_OS_WINDOWS
# include <windows.h>
#endif
namespace testing {
GTEST_DECLARE_bool_(death_test_use_fork);
namespace internal {
GTEST_API_ extern const TypeId kTestTypeIdInGoogleTest;
const char kAlsoRunDisabledTestsFlag[] = "also_run_disabled_tests";
const char kBreakOnFailureFlag[] = "break_on_failure";
const char kCatchExceptionsFlag[] = "catch_exceptions";
const char kColorFlag[] = "color";
const char kFilterFlag[] = "filter";
const char kListTestsFlag[] = "list_tests";
const char kOutputFlag[] = "output";
const char kPrintTimeFlag[] = "print_time";
const char kRandomSeedFlag[] = "random_seed";
const char kRepeatFlag[] = "repeat";
const char kShuffleFlag[] = "shuffle";
const char kStackTraceDepthFlag[] = "stack_trace_depth";
const char kStreamResultToFlag[] = "stream_result_to";
const char kThrowOnFailureFlag[] = "throw_on_failure";
const int kMaxRandomSeed = 99999;
GTEST_API_ extern bool g_help_flag;
GTEST_API_ TimeInMillis GetTimeInMillis();
GTEST_API_ bool ShouldUseColor(bool stdout_is_tty);
GTEST_API_ std::string FormatTimeInMillisAsSeconds(TimeInMillis ms);
GTEST_API_ std::string FormatEpochTimeInMillisAsIso8601(TimeInMillis ms);
GTEST_API_ bool ParseInt32Flag(
    const char* str, const char* flag, Int32* value);
inline int GetRandomSeedFromFlag(Int32 random_seed_flag) {
  const unsigned int raw_seed = (random_seed_flag == 0) ?
      static_cast<unsigned int>(GetTimeInMillis()) :
      static_cast<unsigned int>(random_seed_flag);
  const int normalized_seed =
      static_cast<int>((raw_seed - 1U) %
                       static_cast<unsigned int>(kMaxRandomSeed)) + 1;
  return normalized_seed;
}
inline int GetNextRandomSeed(int seed) {
  GTEST_CHECK_(1 <= seed && seed <= kMaxRandomSeed)
      << "Invalid random seed " << seed << " - must be in [1, "
      << kMaxRandomSeed << "].";
  const int next_seed = seed + 1;
  return (next_seed > kMaxRandomSeed) ? 1 : next_seed;
}
class GTestFlagSaver {
 public:
  GTestFlagSaver() {
    also_run_disabled_tests_ = GTEST_FLAG(also_run_disabled_tests);
    break_on_failure_ = GTEST_FLAG(break_on_failure);
    catch_exceptions_ = GTEST_FLAG(catch_exceptions);
    color_ = GTEST_FLAG(color);
    death_test_style_ = GTEST_FLAG(death_test_style);
    death_test_use_fork_ = GTEST_FLAG(death_test_use_fork);
    filter_ = GTEST_FLAG(filter);
    internal_run_death_test_ = GTEST_FLAG(internal_run_death_test);
    list_tests_ = GTEST_FLAG(list_tests);
    output_ = GTEST_FLAG(output);
    print_time_ = GTEST_FLAG(print_time);
    random_seed_ = GTEST_FLAG(random_seed);
    repeat_ = GTEST_FLAG(repeat);
    shuffle_ = GTEST_FLAG(shuffle);
    stack_trace_depth_ = GTEST_FLAG(stack_trace_depth);
    stream_result_to_ = GTEST_FLAG(stream_result_to);
    throw_on_failure_ = GTEST_FLAG(throw_on_failure);
  }
  ~GTestFlagSaver() {
    GTEST_FLAG(also_run_disabled_tests) = also_run_disabled_tests_;
    GTEST_FLAG(break_on_failure) = break_on_failure_;
    GTEST_FLAG(catch_exceptions) = catch_exceptions_;
    GTEST_FLAG(color) = color_;
    GTEST_FLAG(death_test_style) = death_test_style_;
    GTEST_FLAG(death_test_use_fork) = death_test_use_fork_;
    GTEST_FLAG(filter) = filter_;
    GTEST_FLAG(internal_run_death_test) = internal_run_death_test_;
    GTEST_FLAG(list_tests) = list_tests_;
    GTEST_FLAG(output) = output_;
    GTEST_FLAG(print_time) = print_time_;
    GTEST_FLAG(random_seed) = random_seed_;
    GTEST_FLAG(repeat) = repeat_;
    GTEST_FLAG(shuffle) = shuffle_;
    GTEST_FLAG(stack_trace_depth) = stack_trace_depth_;
    GTEST_FLAG(stream_result_to) = stream_result_to_;
    GTEST_FLAG(throw_on_failure) = throw_on_failure_;
  }
 private:
  bool also_run_disabled_tests_;
  bool break_on_failure_;
  bool catch_exceptions_;
  std::string color_;
  std::string death_test_style_;
  bool death_test_use_fork_;
  std::string filter_;
  std::string internal_run_death_test_;
  bool list_tests_;
  std::string output_;
  bool print_time_;
  internal::Int32 random_seed_;
  internal::Int32 repeat_;
  bool shuffle_;
  internal::Int32 stack_trace_depth_;
  std::string stream_result_to_;
  bool throw_on_failure_;
} GTEST_ATTRIBUTE_UNUSED_;
GTEST_API_ std::string CodePointToUtf8(UInt32 code_point);
GTEST_API_ std::string WideStringToUtf8(const wchar_t* str, int num_chars);
void WriteToShardStatusFileIfNeeded();
GTEST_API_ bool ShouldShard(const char* total_shards_str,
                            const char* shard_index_str,
                            bool in_subprocess_for_death_test);
GTEST_API_ Int32 Int32FromEnvOrDie(const char* env_var, Int32 default_val);
GTEST_API_ bool ShouldRunTestOnShard(
    int total_shards, int shard_index, int test_id);
template <class Container, typename Predicate>
inline int CountIf(const Container& c, Predicate predicate) {
  int count = 0;
  for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it) {
    if (predicate(*it))
      ++count;
  }
  return count;
}
template <class Container, typename Functor>
void ForEach(const Container& c, Functor functor) {
  std::for_each(c.begin(), c.end(), functor);
}
template <typename E>
inline E GetElementOr(const std::vector<E>& v, int i, E default_value) {
  return (i < 0 || i >= static_cast<int>(v.size())) ? default_value : v[i];
}
template <typename E>
void ShuffleRange(internal::Random* random, int begin, int end,
                  std::vector<E>* v) {
  const int size = static_cast<int>(v->size());
  GTEST_CHECK_(0 <= begin && begin <= size)
      << "Invalid shuffle range start " << begin << ": must be in range [0, "
      << size << "].";
  GTEST_CHECK_(begin <= end && end <= size)
      << "Invalid shuffle range finish " << end << ": must be in range ["
      << begin << ", " << size << "].";
  for (int range_width = end - begin; range_width >= 2; range_width--) {
    const int last_in_range = begin + range_width - 1;
    const int selected = begin + random->Generate(range_width);
    std::swap((*v)[selected], (*v)[last_in_range]);
  }
}
template <typename E>
inline void Shuffle(internal::Random* random, std::vector<E>* v) {
  ShuffleRange(random, 0, static_cast<int>(v->size()), v);
}
template <typename T>
static void Delete(T* x) {
  delete x;
}
class TestPropertyKeyIs {
 public:
  explicit TestPropertyKeyIs(const std::string& key) : key_(key) {}
  bool operator()(const TestProperty& test_property) const {
    return test_property.key() == key_;
  }
 private:
  std::string key_;
};
class GTEST_API_ UnitTestOptions {
 public:
  static std::string GetOutputFormat();
  static std::string GetAbsolutePathToOutputFile();
  static bool PatternMatchesString(const char *pattern, const char *str);
  static bool FilterMatchesTest(const std::string &test_case_name,
                                const std::string &test_name);
#if GTEST_OS_WINDOWS
  static int GTestShouldProcessSEH(DWORD exception_code);
#endif
  static bool MatchesFilter(const std::string& name, const char* filter);
};
GTEST_API_ FilePath GetCurrentExecutableName();
class OsStackTraceGetterInterface {
 public:
  OsStackTraceGetterInterface() {}
  virtual ~OsStackTraceGetterInterface() {}
  virtual string CurrentStackTrace(int max_depth, int skip_count) = 0;
  virtual void UponLeavingGTest() = 0;
 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(OsStackTraceGetterInterface);
};
class OsStackTraceGetter : public OsStackTraceGetterInterface {
 public:
  OsStackTraceGetter() : caller_frame_(NULL) {}
  virtual string CurrentStackTrace(int max_depth, int skip_count)
      GTEST_LOCK_EXCLUDED_(mutex_);
  virtual void UponLeavingGTest() GTEST_LOCK_EXCLUDED_(mutex_);
  static const char* const kElidedFramesMarker;
 private:
  Mutex mutex_;
  void* caller_frame_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(OsStackTraceGetter);
};
struct TraceInfo {
  const char* file;
  int line;
  std::string message;
};
class DefaultGlobalTestPartResultReporter
  : public TestPartResultReporterInterface {
 public:
  explicit DefaultGlobalTestPartResultReporter(UnitTestImpl* unit_test);
  virtual void ReportTestPartResult(const TestPartResult& result);
 private:
  UnitTestImpl* const unit_test_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(DefaultGlobalTestPartResultReporter);
};
class DefaultPerThreadTestPartResultReporter
    : public TestPartResultReporterInterface {
 public:
  explicit DefaultPerThreadTestPartResultReporter(UnitTestImpl* unit_test);
  virtual void ReportTestPartResult(const TestPartResult& result);
 private:
  UnitTestImpl* const unit_test_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(DefaultPerThreadTestPartResultReporter);
};
class GTEST_API_ UnitTestImpl {
 public:
  explicit UnitTestImpl(UnitTest* parent);
  virtual ~UnitTestImpl();
  TestPartResultReporterInterface* GetGlobalTestPartResultReporter();
  void SetGlobalTestPartResultReporter(
      TestPartResultReporterInterface* reporter);
  TestPartResultReporterInterface* GetTestPartResultReporterForCurrentThread();
  void SetTestPartResultReporterForCurrentThread(
      TestPartResultReporterInterface* reporter);
  int successful_test_case_count() const;
  int failed_test_case_count() const;
  int total_test_case_count() const;
  int test_case_to_run_count() const;
  int successful_test_count() const;
  int failed_test_count() const;
  int reportable_disabled_test_count() const;
  int disabled_test_count() const;
  int reportable_test_count() const;
  int total_test_count() const;
  int test_to_run_count() const;
  TimeInMillis start_timestamp() const { return start_timestamp_; }
  TimeInMillis elapsed_time() const { return elapsed_time_; }
  bool Passed() const { return !Failed(); }
  bool Failed() const {
    return failed_test_case_count() > 0 || ad_hoc_test_result()->Failed();
  }
  const TestCase* GetTestCase(int i) const {
    const int index = GetElementOr(test_case_indices_, i, -1);
    return index < 0 ? NULL : test_cases_[i];
  }
  TestCase* GetMutableTestCase(int i) {
    const int index = GetElementOr(test_case_indices_, i, -1);
    return index < 0 ? NULL : test_cases_[index];
  }
  TestEventListeners* listeners() { return &listeners_; }
  TestResult* current_test_result();
  const TestResult* ad_hoc_test_result() const { return &ad_hoc_test_result_; }
  void set_os_stack_trace_getter(OsStackTraceGetterInterface* getter);
  OsStackTraceGetterInterface* os_stack_trace_getter();
  std::string CurrentOsStackTraceExceptTop(int skip_count) GTEST_NO_INLINE_;
  TestCase* GetTestCase(const char* test_case_name,
                        const char* type_param,
                        Test::SetUpTestCaseFunc set_up_tc,
                        Test::TearDownTestCaseFunc tear_down_tc);
  void AddTestInfo(Test::SetUpTestCaseFunc set_up_tc,
                   Test::TearDownTestCaseFunc tear_down_tc,
                   TestInfo* test_info) {
    if (original_working_dir_.IsEmpty()) {
      original_working_dir_.Set(FilePath::GetCurrentDir());
      GTEST_CHECK_(!original_working_dir_.IsEmpty())
          << "Failed to get the current working directory.";
    }
    GetTestCase(test_info->test_case_name(),
                test_info->type_param(),
                set_up_tc,
                tear_down_tc)->AddTestInfo(test_info);
  }
#if GTEST_HAS_PARAM_TEST
  internal::ParameterizedTestCaseRegistry& parameterized_test_registry() {
    return parameterized_test_registry_;
  }
#endif
  void set_current_test_case(TestCase* a_current_test_case) {
    current_test_case_ = a_current_test_case;
  }
  void set_current_test_info(TestInfo* a_current_test_info) {
    current_test_info_ = a_current_test_info;
  }
  void RegisterParameterizedTests();
  bool RunAllTests();
  void ClearNonAdHocTestResult() {
    ForEach(test_cases_, TestCase::ClearTestCaseResult);
  }
  void ClearAdHocTestResult() {
    ad_hoc_test_result_.Clear();
  }
  void RecordProperty(const TestProperty& test_property);
  enum ReactionToSharding {
    HONOR_SHARDING_PROTOCOL,
    IGNORE_SHARDING_PROTOCOL
  };
  int FilterTests(ReactionToSharding shard_tests);
  void ListTestsMatchingFilter();
  const TestCase* current_test_case() const { return current_test_case_; }
  TestInfo* current_test_info() { return current_test_info_; }
  const TestInfo* current_test_info() const { return current_test_info_; }
  std::vector<Environment*>& environments() { return environments_; }
  std::vector<TraceInfo>& gtest_trace_stack() {
    return *(gtest_trace_stack_.pointer());
  }
  const std::vector<TraceInfo>& gtest_trace_stack() const {
    return gtest_trace_stack_.get();
  }
#if GTEST_HAS_DEATH_TEST
  void InitDeathTestSubprocessControlInfo() {
    internal_run_death_test_flag_.reset(ParseInternalRunDeathTestFlag());
  }
  const InternalRunDeathTestFlag* internal_run_death_test_flag() const {
    return internal_run_death_test_flag_.get();
  }
  internal::DeathTestFactory* death_test_factory() {
    return death_test_factory_.get();
  }
  void SuppressTestEventsIfInSubprocess();
  friend class ReplaceDeathTestFactory;
#endif
  void ConfigureXmlOutput();
#if GTEST_CAN_STREAM_RESULTS_
  void ConfigureStreamingOutput();
#endif
  void PostFlagParsingInit();
  int random_seed() const { return random_seed_; }
  internal::Random* random() { return &random_; }
  void ShuffleTests();
  void UnshuffleTests();
  bool catch_exceptions() const { return catch_exceptions_; }
 private:
  friend class ::testing::UnitTest;
  void set_catch_exceptions(bool value) { catch_exceptions_ = value; }
  UnitTest* const parent_;
  internal::FilePath original_working_dir_;
  DefaultGlobalTestPartResultReporter default_global_test_part_result_reporter_;
  DefaultPerThreadTestPartResultReporter
      default_per_thread_test_part_result_reporter_;
  TestPartResultReporterInterface* global_test_part_result_repoter_;
  internal::Mutex global_test_part_result_reporter_mutex_;
  internal::ThreadLocal<TestPartResultReporterInterface*>
      per_thread_test_part_result_reporter_;
  std::vector<Environment*> environments_;
  std::vector<TestCase*> test_cases_;
  std::vector<int> test_case_indices_;
#if GTEST_HAS_PARAM_TEST
  internal::ParameterizedTestCaseRegistry parameterized_test_registry_;
  bool parameterized_tests_registered_;
#endif
  int last_death_test_case_;
  TestCase* current_test_case_;
  TestInfo* current_test_info_;
  TestResult ad_hoc_test_result_;
  TestEventListeners listeners_;
  OsStackTraceGetterInterface* os_stack_trace_getter_;
  bool post_flag_parse_init_performed_;
  int random_seed_;
  internal::Random random_;
  TimeInMillis start_timestamp_;
  TimeInMillis elapsed_time_;
#if GTEST_HAS_DEATH_TEST
  internal::scoped_ptr<InternalRunDeathTestFlag> internal_run_death_test_flag_;
  internal::scoped_ptr<internal::DeathTestFactory> death_test_factory_;
#endif
  internal::ThreadLocal<std::vector<TraceInfo> > gtest_trace_stack_;
  bool catch_exceptions_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(UnitTestImpl);
};
inline UnitTestImpl* GetUnitTestImpl() {
  return UnitTest::GetInstance()->impl();
}
#if GTEST_USES_SIMPLE_RE
GTEST_API_ bool IsInSet(char ch, const char* str);
GTEST_API_ bool IsAsciiDigit(char ch);
GTEST_API_ bool IsAsciiPunct(char ch);
GTEST_API_ bool IsRepeat(char ch);
GTEST_API_ bool IsAsciiWhiteSpace(char ch);
GTEST_API_ bool IsAsciiWordChar(char ch);
GTEST_API_ bool IsValidEscape(char ch);
GTEST_API_ bool AtomMatchesChar(bool escaped, char pattern, char ch);
GTEST_API_ bool ValidateRegex(const char* regex);
GTEST_API_ bool MatchRegexAtHead(const char* regex, const char* str);
GTEST_API_ bool MatchRepetitionAndRegexAtHead(
    bool escaped, char ch, char repeat, const char* regex, const char* str);
GTEST_API_ bool MatchRegexAnywhere(const char* regex, const char* str);
#endif
GTEST_API_ void ParseGoogleTestFlagsOnly(int* argc, char** argv);
GTEST_API_ void ParseGoogleTestFlagsOnly(int* argc, wchar_t** argv);
#if GTEST_HAS_DEATH_TEST
GTEST_API_ std::string GetLastErrnoDescription();
# if GTEST_OS_WINDOWS
class AutoHandle {
 public:
  AutoHandle() : handle_(INVALID_HANDLE_VALUE) {}
  explicit AutoHandle(HANDLE handle) : handle_(handle) {}
  ~AutoHandle() { Reset(); }
  HANDLE Get() const { return handle_; }
  void Reset() { Reset(INVALID_HANDLE_VALUE); }
  void Reset(HANDLE handle) {
    if (handle != handle_) {
      if (handle_ != INVALID_HANDLE_VALUE)
        ::CloseHandle(handle_);
      handle_ = handle;
    }
  }
 private:
  HANDLE handle_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(AutoHandle);
};
# endif
template <typename Integer>
bool ParseNaturalNumber(const ::std::string& str, Integer* number) {
  if (str.empty() || !IsDigit(str[0])) {
    return false;
  }
  errno = 0;
  char* end;
# if GTEST_OS_WINDOWS && !defined(__GNUC__)
  typedef unsigned __int64 BiggestConvertible;
  const BiggestConvertible parsed = _strtoui64(str.c_str(), &end, 10);
# else
  typedef unsigned long long BiggestConvertible;
  const BiggestConvertible parsed = strtoull(str.c_str(), &end, 10);
# endif
  const bool parse_success = *end == '\0' && errno == 0;
  GTEST_CHECK_(sizeof(Integer) <= sizeof(parsed));
  const Integer result = static_cast<Integer>(parsed);
  if (parse_success && static_cast<BiggestConvertible>(result) == parsed) {
    *number = result;
    return true;
  }
  return false;
}
#endif
class TestResultAccessor {
 public:
  static void RecordProperty(TestResult* test_result,
                             const std::string& xml_element,
                             const TestProperty& property) {
    test_result->RecordProperty(xml_element, property);
  }
  static void ClearTestPartResults(TestResult* test_result) {
    test_result->ClearTestPartResults();
  }
  static const std::vector<testing::TestPartResult>& test_part_results(
      const TestResult& test_result) {
    return test_result.test_part_results();
  }
};
#if GTEST_CAN_STREAM_RESULTS_
class StreamingListener : public EmptyTestEventListener {
 public:
  class AbstractSocketWriter {
   public:
    virtual ~AbstractSocketWriter() {}
    virtual void Send(const string& message) = 0;
    virtual void CloseConnection() {}
    void SendLn(const string& message) {
      Send(message + "\n");
    }
  };
  class SocketWriter : public AbstractSocketWriter {
   public:
    SocketWriter(const string& host, const string& port)
        : sockfd_(-1), host_name_(host), port_num_(port) {
      MakeConnection();
    }
    virtual ~SocketWriter() {
      if (sockfd_ != -1)
        CloseConnection();
    }
    virtual void Send(const string& message) {
      GTEST_CHECK_(sockfd_ != -1)
          << "Send() can be called only when there is a connection.";
      const int len = static_cast<int>(message.length());
      if (write(sockfd_, message.c_str(), len) != len) {
        GTEST_LOG_(WARNING)
            << "stream_result_to: failed to stream to "
            << host_name_ << ":" << port_num_;
      }
    }
   private:
    void MakeConnection();
    void CloseConnection() {
      GTEST_CHECK_(sockfd_ != -1)
          << "CloseConnection() can be called only when there is a connection.";
      close(sockfd_);
      sockfd_ = -1;
    }
    int sockfd_;
    const string host_name_;
    const string port_num_;
    GTEST_DISALLOW_COPY_AND_ASSIGN_(SocketWriter);
  };
  static string UrlEncode(const char* str);
  StreamingListener(const string& host, const string& port)
      : socket_writer_(new SocketWriter(host, port)) { Start(); }
  explicit StreamingListener(AbstractSocketWriter* socket_writer)
      : socket_writer_(socket_writer) { Start(); }
  void OnTestProgramStart(const UnitTest& ) {
    SendLn("event=TestProgramStart");
  }
  void OnTestProgramEnd(const UnitTest& unit_test) {
    SendLn("event=TestProgramEnd&passed=" + FormatBool(unit_test.Passed()));
    socket_writer_->CloseConnection();
  }
  void OnTestIterationStart(const UnitTest& , int iteration) {
    SendLn("event=TestIterationStart&iteration=" +
           StreamableToString(iteration));
  }
  void OnTestIterationEnd(const UnitTest& unit_test, int ) {
    SendLn("event=TestIterationEnd&passed=" +
           FormatBool(unit_test.Passed()) + "&elapsed_time=" +
           StreamableToString(unit_test.elapsed_time()) + "ms");
  }
  void OnTestCaseStart(const TestCase& test_case) {
    SendLn(std::string("event=TestCaseStart&name=") + test_case.name());
  }
  void OnTestCaseEnd(const TestCase& test_case) {
    SendLn("event=TestCaseEnd&passed=" + FormatBool(test_case.Passed())
           + "&elapsed_time=" + StreamableToString(test_case.elapsed_time())
           + "ms");
  }
  void OnTestStart(const TestInfo& test_info) {
    SendLn(std::string("event=TestStart&name=") + test_info.name());
  }
  void OnTestEnd(const TestInfo& test_info) {
    SendLn("event=TestEnd&passed=" +
           FormatBool((test_info.result())->Passed()) +
           "&elapsed_time=" +
           StreamableToString((test_info.result())->elapsed_time()) + "ms");
  }
  void OnTestPartResult(const TestPartResult& test_part_result) {
    const char* file_name = test_part_result.file_name();
    if (file_name == NULL)
      file_name = "";
    SendLn("event=TestPartResult&file=" + UrlEncode(file_name) +
           "&line=" + StreamableToString(test_part_result.line_number()) +
           "&message=" + UrlEncode(test_part_result.message()));
  }
 private:
  void SendLn(const string& message) { socket_writer_->SendLn(message); }
  void Start() { SendLn("gtest_streaming_protocol_version=1.0"); }
  string FormatBool(bool value) { return value ? "1" : "0"; }
  const scoped_ptr<AbstractSocketWriter> socket_writer_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(StreamingListener);
};
#endif
}
}
#endif
#undef GTEST_IMPLEMENTATION_
#if GTEST_OS_WINDOWS
#define vsnprintf _vsnprintf
#endif
namespace testing {
using internal::CountIf;
using internal::ForEach;
using internal::GetElementOr;
using internal::Shuffle;
static const char kDisableTestFilter[] = "DISABLED_*:*/DISABLED_*";
static const char kDeathTestCaseFilter[] = "*DeathTest:*DeathTest/*";
static const char kUniversalFilter[] = "*";
static const char kDefaultOutputFile[] = "test_detail.xml";
static const char kTestShardIndex[] = "GTEST_SHARD_INDEX";
static const char kTestTotalShards[] = "GTEST_TOTAL_SHARDS";
static const char kTestShardStatusFile[] = "GTEST_SHARD_STATUS_FILE";
namespace internal {
const char kStackTraceMarker[] = "\nStack trace:\n";
bool g_help_flag = false;
}
static const char* GetDefaultFilter() {
  return kUniversalFilter;
}
GTEST_DEFINE_bool_(
    also_run_disabled_tests,
    internal::BoolFromGTestEnv("also_run_disabled_tests", false),
    "Run disabled tests too, in addition to the tests normally being run.");
GTEST_DEFINE_bool_(
    break_on_failure,
    internal::BoolFromGTestEnv("break_on_failure", false),
    "True iff a failed assertion should be a debugger break-point.");
GTEST_DEFINE_bool_(
    catch_exceptions,
    internal::BoolFromGTestEnv("catch_exceptions", true),
    "True iff " GTEST_NAME_
    " should catch exceptions and treat them as test failures.");
GTEST_DEFINE_string_(
    color,
    internal::StringFromGTestEnv("color", "auto"),
    "Whether to use colors in the output.  Valid values: yes, no, "
    "and auto.  'auto' means to use colors if the output is "
    "being sent to a terminal and the TERM environment variable "
    "is set to a terminal type that supports colors.");
GTEST_DEFINE_string_(
    filter,
    internal::StringFromGTestEnv("filter", GetDefaultFilter()),
    "A colon-separated list of glob (not regex) patterns "
    "for filtering the tests to run, optionally followed by a "
    "'-' and a : separated list of negative patterns (tests to "
    "exclude).  A test is run if it matches one of the positive "
    "patterns and does not match any of the negative patterns.");
GTEST_DEFINE_bool_(list_tests, false,
                   "List all tests without running them.");
GTEST_DEFINE_string_(
    output,
    internal::StringFromGTestEnv("output", ""),
    "A format (currently must be \"xml\"), optionally followed "
    "by a colon and an output file name or directory. A directory "
    "is indicated by a trailing pathname separator. "
    "Examples: \"xml:filename.xml\", \"xml::directoryname/\". "
    "If a directory is specified, output files will be created "
    "within that directory, with file-names based on the test "
    "executable's name and, if necessary, made unique by adding "
    "digits.");
GTEST_DEFINE_bool_(
    print_time,
    internal::BoolFromGTestEnv("print_time", true),
    "True iff " GTEST_NAME_
    " should display elapsed time in text output.");
GTEST_DEFINE_int32_(
    random_seed,
    internal::Int32FromGTestEnv("random_seed", 0),
    "Random number seed to use when shuffling test orders.  Must be in range "
    "[1, 99999], or 0 to use a seed based on the current time.");
GTEST_DEFINE_int32_(
    repeat,
    internal::Int32FromGTestEnv("repeat", 1),
    "How many times to repeat each test.  Specify a negative number "
    "for repeating forever.  Useful for shaking out flaky tests.");
GTEST_DEFINE_bool_(
    show_internal_stack_frames, false,
    "True iff " GTEST_NAME_ " should include internal stack frames when "
    "printing test failure stack traces.");
GTEST_DEFINE_bool_(
    shuffle,
    internal::BoolFromGTestEnv("shuffle", false),
    "True iff " GTEST_NAME_
    " should randomize tests' order on every run.");
GTEST_DEFINE_int32_(
    stack_trace_depth,
    internal::Int32FromGTestEnv("stack_trace_depth", kMaxStackTraceDepth),
    "The maximum number of stack frames to print when an "
    "assertion fails.  The valid range is 0 through 100, inclusive.");
GTEST_DEFINE_string_(
    stream_result_to,
    internal::StringFromGTestEnv("stream_result_to", ""),
    "This flag specifies the host name and the port number on which to stream "
    "test results. Example: \"localhost:555\". The flag is effective only on "
    "Linux.");
GTEST_DEFINE_bool_(
    throw_on_failure,
    internal::BoolFromGTestEnv("throw_on_failure", false),
    "When this flag is specified, a failed assertion will throw an exception "
    "if exceptions are enabled or exit the program with a non-zero code "
    "otherwise.");
namespace internal {
UInt32 Random::Generate(UInt32 range) {
  state_ = (1103515245U*state_ + 12345U) % kMaxRange;
  GTEST_CHECK_(range > 0)
      << "Cannot generate a number in the range [0, 0).";
  GTEST_CHECK_(range <= kMaxRange)
      << "Generation of a number in [0, " << range << ") was requested, "
      << "but this can only generate numbers in [0, " << kMaxRange << ").";
  return state_ % range;
}
GTEST_API_ int g_init_gtest_count = 0;
static bool GTestIsInitialized() { return g_init_gtest_count != 0; }
static int SumOverTestCaseList(const std::vector<TestCase*>& case_list,
                               int (TestCase::*method)() const) {
  int sum = 0;
  for (size_t i = 0; i < case_list.size(); i++) {
    sum += (case_list[i]->*method)();
  }
  return sum;
}
static bool TestCasePassed(const TestCase* test_case) {
  return test_case->should_run() && test_case->Passed();
}
static bool TestCaseFailed(const TestCase* test_case) {
  return test_case->should_run() && test_case->Failed();
}
static bool ShouldRunTestCase(const TestCase* test_case) {
  return test_case->should_run();
}
AssertHelper::AssertHelper(TestPartResult::Type type,
                           const char* file,
                           int line,
                           const char* message)
    : data_(new AssertHelperData(type, file, line, message)) {
}
AssertHelper::~AssertHelper() {
  delete data_;
}
void AssertHelper::operator=(const Message& message) const {
  UnitTest::GetInstance()->
    AddTestPartResult(data_->type, data_->file, data_->line,
                      AppendUserMessage(data_->message, message),
                      UnitTest::GetInstance()->impl()
                      ->CurrentOsStackTraceExceptTop(1)
                      );
}
GTEST_API_ GTEST_DEFINE_STATIC_MUTEX_(g_linked_ptr_mutex);
std::string g_executable_path;
FilePath GetCurrentExecutableName() {
  FilePath result;
#if GTEST_OS_WINDOWS
  result.Set(FilePath(g_executable_path).RemoveExtension("exe"));
#else
  result.Set(FilePath(g_executable_path));
#endif
  return result.RemoveDirectoryName();
}
std::string UnitTestOptions::GetOutputFormat() {
  const char* const gtest_output_flag = GTEST_FLAG(output).c_str();
  if (gtest_output_flag == NULL) return std::string("");
  const char* const colon = strchr(gtest_output_flag, ':');
  return (colon == NULL) ?
      std::string(gtest_output_flag) :
      std::string(gtest_output_flag, colon - gtest_output_flag);
}
std::string UnitTestOptions::GetAbsolutePathToOutputFile() {
  const char* const gtest_output_flag = GTEST_FLAG(output).c_str();
  if (gtest_output_flag == NULL)
    return "";
  const char* const colon = strchr(gtest_output_flag, ':');
  if (colon == NULL)
    return internal::FilePath::ConcatPaths(
        internal::FilePath(
            UnitTest::GetInstance()->original_working_dir()),
        internal::FilePath(kDefaultOutputFile)).string();
  internal::FilePath output_name(colon + 1);
  if (!output_name.IsAbsolutePath())
    output_name = internal::FilePath::ConcatPaths(
        internal::FilePath(UnitTest::GetInstance()->original_working_dir()),
        internal::FilePath(colon + 1));
  if (!output_name.IsDirectory())
    return output_name.string();
  internal::FilePath result(internal::FilePath::GenerateUniqueFileName(
      output_name, internal::GetCurrentExecutableName(),
      GetOutputFormat().c_str()));
  return result.string();
}
bool UnitTestOptions::PatternMatchesString(const char *pattern,
                                           const char *str) {
  switch (*pattern) {
    case '\0':
    case ':':
      return *str == '\0';
    case '?':
      return *str != '\0' && PatternMatchesString(pattern + 1, str + 1);
    case '*':
      return (*str != '\0' && PatternMatchesString(pattern, str + 1)) ||
          PatternMatchesString(pattern + 1, str);
    default:
      return *pattern == *str &&
          PatternMatchesString(pattern + 1, str + 1);
  }
}
bool UnitTestOptions::MatchesFilter(
    const std::string& name, const char* filter) {
  const char *cur_pattern = filter;
  for (;;) {
    if (PatternMatchesString(cur_pattern, name.c_str())) {
      return true;
    }
    cur_pattern = strchr(cur_pattern, ':');
    if (cur_pattern == NULL) {
      return false;
    }
    cur_pattern++;
  }
}
bool UnitTestOptions::FilterMatchesTest(const std::string &test_case_name,
                                        const std::string &test_name) {
  const std::string& full_name = test_case_name + "." + test_name.c_str();
  const char* const p = GTEST_FLAG(filter).c_str();
  const char* const dash = strchr(p, '-');
  std::string positive;
  std::string negative;
  if (dash == NULL) {
    positive = GTEST_FLAG(filter).c_str();
    negative = "";
  } else {
    positive = std::string(p, dash);
    negative = std::string(dash + 1);
    if (positive.empty()) {
      positive = kUniversalFilter;
    }
  }
  return (MatchesFilter(full_name, positive.c_str()) &&
          !MatchesFilter(full_name, negative.c_str()));
}
#if GTEST_HAS_SEH
int UnitTestOptions::GTestShouldProcessSEH(DWORD exception_code) {
  const DWORD kCxxExceptionCode = 0xe06d7363;
  bool should_handle = true;
  if (!GTEST_FLAG(catch_exceptions))
    should_handle = false;
  else if (exception_code == EXCEPTION_BREAKPOINT)
    should_handle = false;
  else if (exception_code == kCxxExceptionCode)
    should_handle = false;
  return should_handle ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}
#endif
}
ScopedFakeTestPartResultReporter::ScopedFakeTestPartResultReporter(
    TestPartResultArray* result)
    : intercept_mode_(INTERCEPT_ONLY_CURRENT_THREAD),
      result_(result) {
  Init();
}
ScopedFakeTestPartResultReporter::ScopedFakeTestPartResultReporter(
    InterceptMode intercept_mode, TestPartResultArray* result)
    : intercept_mode_(intercept_mode),
      result_(result) {
  Init();
}
void ScopedFakeTestPartResultReporter::Init() {
  internal::UnitTestImpl* const impl = internal::GetUnitTestImpl();
  if (intercept_mode_ == INTERCEPT_ALL_THREADS) {
    old_reporter_ = impl->GetGlobalTestPartResultReporter();
    impl->SetGlobalTestPartResultReporter(this);
  } else {
    old_reporter_ = impl->GetTestPartResultReporterForCurrentThread();
    impl->SetTestPartResultReporterForCurrentThread(this);
  }
}
ScopedFakeTestPartResultReporter::~ScopedFakeTestPartResultReporter() {
  internal::UnitTestImpl* const impl = internal::GetUnitTestImpl();
  if (intercept_mode_ == INTERCEPT_ALL_THREADS) {
    impl->SetGlobalTestPartResultReporter(old_reporter_);
  } else {
    impl->SetTestPartResultReporterForCurrentThread(old_reporter_);
  }
}
void ScopedFakeTestPartResultReporter::ReportTestPartResult(
    const TestPartResult& result) {
  result_->Append(result);
}
namespace internal {
TypeId GetTestTypeId() {
  return GetTypeId<Test>();
}
extern const TypeId kTestTypeIdInGoogleTest = GetTestTypeId();
AssertionResult HasOneFailure(const char* ,
                              const char* ,
                              const char* ,
                              const TestPartResultArray& results,
                              TestPartResult::Type type,
                              const string& substr) {
  const std::string expected(type == TestPartResult::kFatalFailure ?
                        "1 fatal failure" :
                        "1 non-fatal failure");
  Message msg;
  if (results.size() != 1) {
    msg << "Expected: " << expected << "\n"
        << "  Actual: " << results.size() << " failures";
    for (int i = 0; i < results.size(); i++) {
      msg << "\n" << results.GetTestPartResult(i);
    }
    return AssertionFailure() << msg;
  }
  const TestPartResult& r = results.GetTestPartResult(0);
  if (r.type() != type) {
    return AssertionFailure() << "Expected: " << expected << "\n"
                              << "  Actual:\n"
                              << r;
  }
  if (strstr(r.message(), substr.c_str()) == NULL) {
    return AssertionFailure() << "Expected: " << expected << " containing \""
                              << substr << "\"\n"
                              << "  Actual:\n"
                              << r;
  }
  return AssertionSuccess();
}
SingleFailureChecker:: SingleFailureChecker(
    const TestPartResultArray* results,
    TestPartResult::Type type,
    const string& substr)
    : results_(results),
      type_(type),
      substr_(substr) {}
SingleFailureChecker::~SingleFailureChecker() {
  EXPECT_PRED_FORMAT3(HasOneFailure, *results_, type_, substr_);
}
DefaultGlobalTestPartResultReporter::DefaultGlobalTestPartResultReporter(
    UnitTestImpl* unit_test) : unit_test_(unit_test) {}
void DefaultGlobalTestPartResultReporter::ReportTestPartResult(
    const TestPartResult& result) {
  unit_test_->current_test_result()->AddTestPartResult(result);
  unit_test_->listeners()->repeater()->OnTestPartResult(result);
}
DefaultPerThreadTestPartResultReporter::DefaultPerThreadTestPartResultReporter(
    UnitTestImpl* unit_test) : unit_test_(unit_test) {}
void DefaultPerThreadTestPartResultReporter::ReportTestPartResult(
    const TestPartResult& result) {
  unit_test_->GetGlobalTestPartResultReporter()->ReportTestPartResult(result);
}
TestPartResultReporterInterface*
UnitTestImpl::GetGlobalTestPartResultReporter() {
  internal::MutexLock lock(&global_test_part_result_reporter_mutex_);
  return global_test_part_result_repoter_;
}
void UnitTestImpl::SetGlobalTestPartResultReporter(
    TestPartResultReporterInterface* reporter) {
  internal::MutexLock lock(&global_test_part_result_reporter_mutex_);
  global_test_part_result_repoter_ = reporter;
}
TestPartResultReporterInterface*
UnitTestImpl::GetTestPartResultReporterForCurrentThread() {
  return per_thread_test_part_result_reporter_.get();
}
void UnitTestImpl::SetTestPartResultReporterForCurrentThread(
    TestPartResultReporterInterface* reporter) {
  per_thread_test_part_result_reporter_.set(reporter);
}
int UnitTestImpl::successful_test_case_count() const {
  return CountIf(test_cases_, TestCasePassed);
}
int UnitTestImpl::failed_test_case_count() const {
  return CountIf(test_cases_, TestCaseFailed);
}
int UnitTestImpl::total_test_case_count() const {
  return static_cast<int>(test_cases_.size());
}
int UnitTestImpl::test_case_to_run_count() const {
  return CountIf(test_cases_, ShouldRunTestCase);
}
int UnitTestImpl::successful_test_count() const {
  return SumOverTestCaseList(test_cases_, &TestCase::successful_test_count);
}
int UnitTestImpl::failed_test_count() const {
  return SumOverTestCaseList(test_cases_, &TestCase::failed_test_count);
}
int UnitTestImpl::reportable_disabled_test_count() const {
  return SumOverTestCaseList(test_cases_,
                             &TestCase::reportable_disabled_test_count);
}
int UnitTestImpl::disabled_test_count() const {
  return SumOverTestCaseList(test_cases_, &TestCase::disabled_test_count);
}
int UnitTestImpl::reportable_test_count() const {
  return SumOverTestCaseList(test_cases_, &TestCase::reportable_test_count);
}
int UnitTestImpl::total_test_count() const {
  return SumOverTestCaseList(test_cases_, &TestCase::total_test_count);
}
int UnitTestImpl::test_to_run_count() const {
  return SumOverTestCaseList(test_cases_, &TestCase::test_to_run_count);
}
std::string UnitTestImpl::CurrentOsStackTraceExceptTop(int skip_count) {
  (void)skip_count;
  return "";
}
TimeInMillis GetTimeInMillis() {
#if GTEST_OS_WINDOWS_MOBILE || defined(__BORLANDC__)
  const TimeInMillis kJavaEpochToWinFileTimeDelta =
    static_cast<TimeInMillis>(116444736UL) * 100000UL;
  const DWORD kTenthMicrosInMilliSecond = 10000;
  SYSTEMTIME now_systime;
  FILETIME now_filetime;
  ULARGE_INTEGER now_int64;
  GetSystemTime(&now_systime);
  if (SystemTimeToFileTime(&now_systime, &now_filetime)) {
    now_int64.LowPart = now_filetime.dwLowDateTime;
    now_int64.HighPart = now_filetime.dwHighDateTime;
    now_int64.QuadPart = (now_int64.QuadPart / kTenthMicrosInMilliSecond) -
      kJavaEpochToWinFileTimeDelta;
    return now_int64.QuadPart;
  }
  return 0;
#elif GTEST_OS_WINDOWS && !GTEST_HAS_GETTIMEOFDAY_
  __timeb64 now;
# ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
  _ftime64(&now);
#pragma warning(pop)
# else
  _ftime64(&now);
# endif
  return static_cast<TimeInMillis>(now.time) * 1000 + now.millitm;
#elif GTEST_HAS_GETTIMEOFDAY_
  struct timeval now;
  gettimeofday(&now, NULL);
  return static_cast<TimeInMillis>(now.tv_sec) * 1000 + now.tv_usec / 1000;
#else
# error "Don't know how to get the current time on your system."
#endif
}
#if GTEST_OS_WINDOWS_MOBILE
LPCWSTR String::AnsiToUtf16(const char* ansi) {
  if (!ansi) return NULL;
  const int length = strlen(ansi);
  const int unicode_length =
      MultiByteToWideChar(CP_ACP, 0, ansi, length,
                          NULL, 0);
  WCHAR* unicode = new WCHAR[unicode_length + 1];
  MultiByteToWideChar(CP_ACP, 0, ansi, length,
                      unicode, unicode_length);
  unicode[unicode_length] = 0;
  return unicode;
}
const char* String::Utf16ToAnsi(LPCWSTR utf16_str) {
  if (!utf16_str) return NULL;
  const int ansi_length =
      WideCharToMultiByte(CP_ACP, 0, utf16_str, -1,
                          NULL, 0, NULL, NULL);
  char* ansi = new char[ansi_length + 1];
  WideCharToMultiByte(CP_ACP, 0, utf16_str, -1,
                      ansi, ansi_length, NULL, NULL);
  ansi[ansi_length] = 0;
  return ansi;
}
#endif
bool String::CStringEquals(const char * lhs, const char * rhs) {
  if ( lhs == NULL ) return rhs == NULL;
  if ( rhs == NULL ) return false;
  return strcmp(lhs, rhs) == 0;
}
#if GTEST_HAS_STD_WSTRING || GTEST_HAS_GLOBAL_WSTRING
static void StreamWideCharsToMessage(const wchar_t* wstr, size_t length,
                                     Message* msg) {
  for (size_t i = 0; i != length; ) {
    if (wstr[i] != L'\0') {
      *msg << WideStringToUtf8(wstr + i, static_cast<int>(length - i));
      while (i != length && wstr[i] != L'\0')
        i++;
    } else {
      *msg << '\0';
      i++;
    }
  }
}
#endif
}
Message::Message() : ss_(new ::std::stringstream) {
  *ss_ << std::setprecision(std::numeric_limits<double>::digits10 + 2);
}
Message& Message::operator <<(const wchar_t* wide_c_str) {
  return *this << internal::String::ShowWideCString(wide_c_str);
}
Message& Message::operator <<(wchar_t* wide_c_str) {
  return *this << internal::String::ShowWideCString(wide_c_str);
}
#if GTEST_HAS_STD_WSTRING
Message& Message::operator <<(const ::std::wstring& wstr) {
  internal::StreamWideCharsToMessage(wstr.c_str(), wstr.length(), this);
  return *this;
}
#endif
#if GTEST_HAS_GLOBAL_WSTRING
Message& Message::operator <<(const ::wstring& wstr) {
  internal::StreamWideCharsToMessage(wstr.c_str(), wstr.length(), this);
  return *this;
}
#endif
std::string Message::GetString() const {
  return internal::StringStreamToString(ss_.get());
}
AssertionResult::AssertionResult(const AssertionResult& other)
    : success_(other.success_),
      message_(other.message_.get() != NULL ?
               new ::std::string(*other.message_) :
               static_cast< ::std::string*>(NULL)) {
}
AssertionResult AssertionResult::operator!() const {
  AssertionResult negation(!success_);
  if (message_.get() != NULL)
    negation << *message_;
  return negation;
}
AssertionResult AssertionSuccess() {
  return AssertionResult(true);
}
AssertionResult AssertionFailure() {
  return AssertionResult(false);
}
AssertionResult AssertionFailure(const Message& message) {
  return AssertionFailure() << message;
}
namespace internal {
AssertionResult EqFailure(const char* expected_expression,
                          const char* actual_expression,
                          const std::string& expected_value,
                          const std::string& actual_value,
                          bool ignoring_case) {
  Message msg;
  msg << "Value of: " << actual_expression;
  if (actual_value != actual_expression) {
    msg << "\n  Actual: " << actual_value;
  }
  msg << "\nExpected: " << expected_expression;
  if (ignoring_case) {
    msg << " (ignoring case)";
  }
  if (expected_value != expected_expression) {
    msg << "\nWhich is: " << expected_value;
  }
  return AssertionFailure() << msg;
}
std::string GetBoolAssertionFailureMessage(
    const AssertionResult& assertion_result,
    const char* expression_text,
    const char* actual_predicate_value,
    const char* expected_predicate_value) {
  const char* actual_message = assertion_result.message();
  Message msg;
  msg << "Value of: " << expression_text
      << "\n  Actual: " << actual_predicate_value;
  if (actual_message[0] != '\0')
    msg << " (" << actual_message << ")";
  msg << "\nExpected: " << expected_predicate_value;
  return msg.GetString();
}
AssertionResult DoubleNearPredFormat(const char* expr1,
                                     const char* expr2,
                                     const char* abs_error_expr,
                                     double val1,
                                     double val2,
                                     double abs_error) {
  const double diff = fabs(val1 - val2);
  if (diff <= abs_error) return AssertionSuccess();
  return AssertionFailure()
      << "The difference between " << expr1 << " and " << expr2
      << " is " << diff << ", which exceeds " << abs_error_expr << ", where\n"
      << expr1 << " evaluates to " << val1 << ",\n"
      << expr2 << " evaluates to " << val2 << ", and\n"
      << abs_error_expr << " evaluates to " << abs_error << ".";
}
template <typename RawType>
AssertionResult FloatingPointLE(const char* expr1,
                                const char* expr2,
                                RawType val1,
                                RawType val2) {
  if (val1 < val2) {
    return AssertionSuccess();
  }
  const FloatingPoint<RawType> lhs(val1), rhs(val2);
  if (lhs.AlmostEquals(rhs)) {
    return AssertionSuccess();
  }
  ::std::stringstream val1_ss;
  val1_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2)
          << val1;
  ::std::stringstream val2_ss;
  val2_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2)
          << val2;
  return AssertionFailure()
      << "Expected: (" << expr1 << ") <= (" << expr2 << ")\n"
      << "  Actual: " << StringStreamToString(&val1_ss) << " vs "
      << StringStreamToString(&val2_ss);
}
}
AssertionResult FloatLE(const char* expr1, const char* expr2,
                        float val1, float val2) {
  return internal::FloatingPointLE<float>(expr1, expr2, val1, val2);
}
AssertionResult DoubleLE(const char* expr1, const char* expr2,
                         double val1, double val2) {
  return internal::FloatingPointLE<double>(expr1, expr2, val1, val2);
}
namespace internal {
AssertionResult CmpHelperEQ(const char* expected_expression,
                            const char* actual_expression,
                            BiggestInt expected,
                            BiggestInt actual) {
  if (expected == actual) {
    return AssertionSuccess();
  }
  return EqFailure(expected_expression,
                   actual_expression,
                   FormatForComparisonFailureMessage(expected, actual),
                   FormatForComparisonFailureMessage(actual, expected),
                   false);
}
#define GTEST_IMPL_CMP_HELPER_(op_name,op) \
AssertionResult CmpHelper##op_name(const char* expr1, const char* expr2, \
                                   BiggestInt val1, BiggestInt val2) {\
  if (val1 op val2) {\
    return AssertionSuccess();\
  } else {\
    return AssertionFailure() \
        << "Expected: (" << expr1 << ") " #op " (" << expr2\
        << "), actual: " << FormatForComparisonFailureMessage(val1, val2)\
        << " vs " << FormatForComparisonFailureMessage(val2, val1);\
  }\
}
GTEST_IMPL_CMP_HELPER_(NE, !=)
GTEST_IMPL_CMP_HELPER_(LE, <=)
GTEST_IMPL_CMP_HELPER_(LT, < )
GTEST_IMPL_CMP_HELPER_(GE, >=)
GTEST_IMPL_CMP_HELPER_(GT, > )
#undef GTEST_IMPL_CMP_HELPER_
AssertionResult CmpHelperSTREQ(const char* expected_expression,
                               const char* actual_expression,
                               const char* expected,
                               const char* actual) {
  if (String::CStringEquals(expected, actual)) {
    return AssertionSuccess();
  }
  return EqFailure(expected_expression,
                   actual_expression,
                   PrintToString(expected),
                   PrintToString(actual),
                   false);
}
AssertionResult CmpHelperSTRCASEEQ(const char* expected_expression,
                                   const char* actual_expression,
                                   const char* expected,
                                   const char* actual) {
  if (String::CaseInsensitiveCStringEquals(expected, actual)) {
    return AssertionSuccess();
  }
  return EqFailure(expected_expression,
                   actual_expression,
                   PrintToString(expected),
                   PrintToString(actual),
                   true);
}
AssertionResult CmpHelperSTRNE(const char* s1_expression,
                               const char* s2_expression,
                               const char* s1,
                               const char* s2) {
  if (!String::CStringEquals(s1, s2)) {
    return AssertionSuccess();
  } else {
    return AssertionFailure() << "Expected: (" << s1_expression << ") != ("
                              << s2_expression << "), actual: \""
                              << s1 << "\" vs \"" << s2 << "\"";
  }
}
AssertionResult CmpHelperSTRCASENE(const char* s1_expression,
                                   const char* s2_expression,
                                   const char* s1,
                                   const char* s2) {
  if (!String::CaseInsensitiveCStringEquals(s1, s2)) {
    return AssertionSuccess();
  } else {
    return AssertionFailure()
        << "Expected: (" << s1_expression << ") != ("
        << s2_expression << ") (ignoring case), actual: \""
        << s1 << "\" vs \"" << s2 << "\"";
  }
}
}
namespace {
bool IsSubstringPred(const char* needle, const char* haystack) {
  if (needle == NULL || haystack == NULL)
    return needle == haystack;
  return strstr(haystack, needle) != NULL;
}
bool IsSubstringPred(const wchar_t* needle, const wchar_t* haystack) {
  if (needle == NULL || haystack == NULL)
    return needle == haystack;
  return wcsstr(haystack, needle) != NULL;
}
template <typename StringType>
bool IsSubstringPred(const StringType& needle,
                     const StringType& haystack) {
  return haystack.find(needle) != StringType::npos;
}
template <typename StringType>
AssertionResult IsSubstringImpl(
    bool expected_to_be_substring,
    const char* needle_expr, const char* haystack_expr,
    const StringType& needle, const StringType& haystack) {
  if (IsSubstringPred(needle, haystack) == expected_to_be_substring)
    return AssertionSuccess();
  const bool is_wide_string = sizeof(needle[0]) > 1;
  const char* const begin_string_quote = is_wide_string ? "L\"" : "\"";
  return AssertionFailure()
      << "Value of: " << needle_expr << "\n"
      << "  Actual: " << begin_string_quote << needle << "\"\n"
      << "Expected: " << (expected_to_be_substring ? "" : "not ")
      << "a substring of " << haystack_expr << "\n"
      << "Which is: " << begin_string_quote << haystack << "\"";
}
}
AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const char* needle, const char* haystack) {
  return IsSubstringImpl(true, needle_expr, haystack_expr, needle, haystack);
}
AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const wchar_t* needle, const wchar_t* haystack) {
  return IsSubstringImpl(true, needle_expr, haystack_expr, needle, haystack);
}
AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const char* needle, const char* haystack) {
  return IsSubstringImpl(false, needle_expr, haystack_expr, needle, haystack);
}
AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const wchar_t* needle, const wchar_t* haystack) {
  return IsSubstringImpl(false, needle_expr, haystack_expr, needle, haystack);
}
AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::string& needle, const ::std::string& haystack) {
  return IsSubstringImpl(true, needle_expr, haystack_expr, needle, haystack);
}
AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::string& needle, const ::std::string& haystack) {
  return IsSubstringImpl(false, needle_expr, haystack_expr, needle, haystack);
}
#if GTEST_HAS_STD_WSTRING
AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::wstring& needle, const ::std::wstring& haystack) {
  return IsSubstringImpl(true, needle_expr, haystack_expr, needle, haystack);
}
AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::wstring& needle, const ::std::wstring& haystack) {
  return IsSubstringImpl(false, needle_expr, haystack_expr, needle, haystack);
}
#endif
namespace internal {
#if GTEST_OS_WINDOWS
namespace {
AssertionResult HRESULTFailureHelper(const char* expr,
                                     const char* expected,
                                     long hr) {
# if GTEST_OS_WINDOWS_MOBILE
  const char error_text[] = "";
# else
  const DWORD kFlags = FORMAT_MESSAGE_FROM_SYSTEM |
                       FORMAT_MESSAGE_IGNORE_INSERTS;
  const DWORD kBufSize = 4096;
  char error_text[kBufSize] = { '\0' };
  DWORD message_length = ::FormatMessageA(kFlags,
                                          0,
                                          hr,
                                          0,
                                          error_text,
                                          kBufSize,
                                          NULL);
  for (; message_length && IsSpace(error_text[message_length - 1]);
          --message_length) {
    error_text[message_length - 1] = '\0';
  }
# endif
  const std::string error_hex("0x" + String::FormatHexInt(hr));
  return ::testing::AssertionFailure()
      << "Expected: " << expr << " " << expected << ".\n"
      << "  Actual: " << error_hex << " " << error_text << "\n";
}
}
AssertionResult IsHRESULTSuccess(const char* expr, long hr) {
  if (SUCCEEDED(hr)) {
    return AssertionSuccess();
  }
  return HRESULTFailureHelper(expr, "succeeds", hr);
}
AssertionResult IsHRESULTFailure(const char* expr, long hr) {
  if (FAILED(hr)) {
    return AssertionSuccess();
  }
  return HRESULTFailureHelper(expr, "fails", hr);
}
#endif
const UInt32 kMaxCodePoint1 = (static_cast<UInt32>(1) << 7) - 1;
const UInt32 kMaxCodePoint2 = (static_cast<UInt32>(1) << (5 + 6)) - 1;
const UInt32 kMaxCodePoint3 = (static_cast<UInt32>(1) << (4 + 2*6)) - 1;
const UInt32 kMaxCodePoint4 = (static_cast<UInt32>(1) << (3 + 3*6)) - 1;
inline UInt32 ChopLowBits(UInt32* bits, int n) {
  const UInt32 low_bits = *bits & ((static_cast<UInt32>(1) << n) - 1);
  *bits >>= n;
  return low_bits;
}
std::string CodePointToUtf8(UInt32 code_point) {
  if (code_point > kMaxCodePoint4) {
    return "(Invalid Unicode 0x" + String::FormatHexInt(code_point) + ")";
  }
  char str[5];
  if (code_point <= kMaxCodePoint1) {
    str[1] = '\0';
    str[0] = static_cast<char>(code_point);
  } else if (code_point <= kMaxCodePoint2) {
    str[2] = '\0';
    str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));
    str[0] = static_cast<char>(0xC0 | code_point);
  } else if (code_point <= kMaxCodePoint3) {
    str[3] = '\0';
    str[2] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));
    str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));
    str[0] = static_cast<char>(0xE0 | code_point);
  } else {
    str[4] = '\0';
    str[3] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));
    str[2] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));
    str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));
    str[0] = static_cast<char>(0xF0 | code_point);
  }
  return str;
}
inline bool IsUtf16SurrogatePair(wchar_t first, wchar_t second) {
  return sizeof(wchar_t) == 2 &&
      (first & 0xFC00) == 0xD800 && (second & 0xFC00) == 0xDC00;
}
inline UInt32 CreateCodePointFromUtf16SurrogatePair(wchar_t first,
                                                    wchar_t second) {
  const UInt32 mask = (1 << 10) - 1;
  return (sizeof(wchar_t) == 2) ?
      (((first & mask) << 10) | (second & mask)) + 0x10000 :
      static_cast<UInt32>(first);
}
std::string WideStringToUtf8(const wchar_t* str, int num_chars) {
  if (num_chars == -1)
    num_chars = static_cast<int>(wcslen(str));
  ::std::stringstream stream;
  for (int i = 0; i < num_chars; ++i) {
    UInt32 unicode_code_point;
    if (str[i] == L'\0') {
      break;
    } else if (i + 1 < num_chars && IsUtf16SurrogatePair(str[i], str[i + 1])) {
      unicode_code_point = CreateCodePointFromUtf16SurrogatePair(str[i],
                                                                 str[i + 1]);
      i++;
    } else {
      unicode_code_point = static_cast<UInt32>(str[i]);
    }
    stream << CodePointToUtf8(unicode_code_point);
  }
  return StringStreamToString(&stream);
}
std::string String::ShowWideCString(const wchar_t * wide_c_str) {
  if (wide_c_str == NULL) return "(null)";
  return internal::WideStringToUtf8(wide_c_str, -1);
}
bool String::WideCStringEquals(const wchar_t * lhs, const wchar_t * rhs) {
  if (lhs == NULL) return rhs == NULL;
  if (rhs == NULL) return false;
  return wcscmp(lhs, rhs) == 0;
}
AssertionResult CmpHelperSTREQ(const char* expected_expression,
                               const char* actual_expression,
                               const wchar_t* expected,
                               const wchar_t* actual) {
  if (String::WideCStringEquals(expected, actual)) {
    return AssertionSuccess();
  }
  return EqFailure(expected_expression,
                   actual_expression,
                   PrintToString(expected),
                   PrintToString(actual),
                   false);
}
AssertionResult CmpHelperSTRNE(const char* s1_expression,
                               const char* s2_expression,
                               const wchar_t* s1,
                               const wchar_t* s2) {
  if (!String::WideCStringEquals(s1, s2)) {
    return AssertionSuccess();
  }
  return AssertionFailure() << "Expected: (" << s1_expression << ") != ("
                            << s2_expression << "), actual: "
                            << PrintToString(s1)
                            << " vs " << PrintToString(s2);
}
bool String::CaseInsensitiveCStringEquals(const char * lhs, const char * rhs) {
  if (lhs == NULL)
    return rhs == NULL;
  if (rhs == NULL)
    return false;
  return posix::StrCaseCmp(lhs, rhs) == 0;
}
bool String::CaseInsensitiveWideCStringEquals(const wchar_t* lhs,
                                              const wchar_t* rhs) {
  if (lhs == NULL) return rhs == NULL;
  if (rhs == NULL) return false;
#if GTEST_OS_WINDOWS
  return _wcsicmp(lhs, rhs) == 0;
#elif GTEST_OS_LINUX && !GTEST_OS_LINUX_ANDROID
  return wcscasecmp(lhs, rhs) == 0;
#else
  wint_t left, right;
  do {
    left = towlower(*lhs++);
    right = towlower(*rhs++);
  } while (left && left == right);
  return left == right;
#endif
}
bool String::EndsWithCaseInsensitive(
    const std::string& str, const std::string& suffix) {
  const size_t str_len = str.length();
  const size_t suffix_len = suffix.length();
  return (str_len >= suffix_len) &&
         CaseInsensitiveCStringEquals(str.c_str() + str_len - suffix_len,
                                      suffix.c_str());
}
std::string String::FormatIntWidth2(int value) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << value;
  return ss.str();
}
std::string String::FormatHexInt(int value) {
  std::stringstream ss;
  ss << std::hex << std::uppercase << value;
  return ss.str();
}
std::string String::FormatByte(unsigned char value) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
     << static_cast<unsigned int>(value);
  return ss.str();
}
std::string StringStreamToString(::std::stringstream* ss) {
  const ::std::string& str = ss->str();
  const char* const start = str.c_str();
  const char* const end = start + str.length();
  std::string result;
  result.reserve(2 * (end - start));
  for (const char* ch = start; ch != end; ++ch) {
    if (*ch == '\0') {
      result += "\\0";
    } else {
      result += *ch;
    }
  }
  return result;
}
std::string AppendUserMessage(const std::string& gtest_msg,
                              const Message& user_msg) {
  const std::string user_msg_string = user_msg.GetString();
  if (user_msg_string.empty()) {
    return gtest_msg;
  }
  return gtest_msg + "\n" + user_msg_string;
}
}
TestResult::TestResult()
    : death_test_count_(0),
      elapsed_time_(0) {
}
TestResult::~TestResult() {
}
const TestPartResult& TestResult::GetTestPartResult(int i) const {
  if (i < 0 || i >= total_part_count())
    internal::posix::Abort();
  return test_part_results_.at(i);
}
const TestProperty& TestResult::GetTestProperty(int i) const {
  if (i < 0 || i >= test_property_count())
    internal::posix::Abort();
  return test_properties_.at(i);
}
void TestResult::ClearTestPartResults() {
  test_part_results_.clear();
}
void TestResult::AddTestPartResult(const TestPartResult& test_part_result) {
  test_part_results_.push_back(test_part_result);
}
void TestResult::RecordProperty(const std::string& xml_element,
                                const TestProperty& test_property) {
  if (!ValidateTestProperty(xml_element, test_property)) {
    return;
  }
  internal::MutexLock lock(&test_properites_mutex_);
  const std::vector<TestProperty>::iterator property_with_matching_key =
      std::find_if(test_properties_.begin(), test_properties_.end(),
                   internal::TestPropertyKeyIs(test_property.key()));
  if (property_with_matching_key == test_properties_.end()) {
    test_properties_.push_back(test_property);
    return;
  }
  property_with_matching_key->SetValue(test_property.value());
}
static const char* const kReservedTestSuitesAttributes[] = {
  "disabled",
  "errors",
  "failures",
  "name",
  "random_seed",
  "tests",
  "time",
  "timestamp"
};
static const char* const kReservedTestSuiteAttributes[] = {
  "disabled",
  "errors",
  "failures",
  "name",
  "tests",
  "time"
};
static const char* const kReservedTestCaseAttributes[] = {
  "classname",
  "name",
  "status",
  "time",
  "type_param",
  "value_param"
};
template <int kSize>
std::vector<std::string> ArrayAsVector(const char* const (&array)[kSize]) {
  return std::vector<std::string>(array, array + kSize);
}
static std::vector<std::string> GetReservedAttributesForElement(
    const std::string& xml_element) {
  if (xml_element == "testsuites") {
    return ArrayAsVector(kReservedTestSuitesAttributes);
  } else if (xml_element == "testsuite") {
    return ArrayAsVector(kReservedTestSuiteAttributes);
  } else if (xml_element == "testcase") {
    return ArrayAsVector(kReservedTestCaseAttributes);
  } else {
    GTEST_CHECK_(false) << "Unrecognized xml_element provided: " << xml_element;
  }
  return std::vector<std::string>();
}
static std::string FormatWordList(const std::vector<std::string>& words) {
  Message word_list;
  for (size_t i = 0; i < words.size(); ++i) {
    if (i > 0 && words.size() > 2) {
      word_list << ", ";
    }
    if (i == words.size() - 1) {
      word_list << "and ";
    }
    word_list << "'" << words[i] << "'";
  }
  return word_list.GetString();
}
bool ValidateTestPropertyName(const std::string& property_name,
                              const std::vector<std::string>& reserved_names) {
  if (std::find(reserved_names.begin(), reserved_names.end(), property_name) !=
          reserved_names.end()) {
    ADD_FAILURE() << "Reserved key used in RecordProperty(): " << property_name
                  << " (" << FormatWordList(reserved_names)
                  << " are reserved by " << GTEST_NAME_ << ")";
    return false;
  }
  return true;
}
bool TestResult::ValidateTestProperty(const std::string& xml_element,
                                      const TestProperty& test_property) {
  return ValidateTestPropertyName(test_property.key(),
                                  GetReservedAttributesForElement(xml_element));
}
void TestResult::Clear() {
  test_part_results_.clear();
  test_properties_.clear();
  death_test_count_ = 0;
  elapsed_time_ = 0;
}
bool TestResult::Failed() const {
  for (int i = 0; i < total_part_count(); ++i) {
    if (GetTestPartResult(i).failed())
      return true;
  }
  return false;
}
static bool TestPartFatallyFailed(const TestPartResult& result) {
  return result.fatally_failed();
}
bool TestResult::HasFatalFailure() const {
  return CountIf(test_part_results_, TestPartFatallyFailed) > 0;
}
static bool TestPartNonfatallyFailed(const TestPartResult& result) {
  return result.nonfatally_failed();
}
bool TestResult::HasNonfatalFailure() const {
  return CountIf(test_part_results_, TestPartNonfatallyFailed) > 0;
}
int TestResult::total_part_count() const {
  return static_cast<int>(test_part_results_.size());
}
int TestResult::test_property_count() const {
  return static_cast<int>(test_properties_.size());
}
Test::Test()
    : gtest_flag_saver_(new internal::GTestFlagSaver) {
}
Test::~Test() {
  delete gtest_flag_saver_;
}
void Test::SetUp() {
}
void Test::TearDown() {
}
void Test::RecordProperty(const std::string& key, const std::string& value) {
  UnitTest::GetInstance()->RecordProperty(key, value);
}
void Test::RecordProperty(const std::string& key, int value) {
  Message value_message;
  value_message << value;
  RecordProperty(key, value_message.GetString().c_str());
}
namespace internal {
void ReportFailureInUnknownLocation(TestPartResult::Type result_type,
                                    const std::string& message) {
  UnitTest::GetInstance()->AddTestPartResult(
      result_type,
      NULL,
      -1,
      message,
      "");
}
}
bool Test::HasSameFixtureClass() {
  internal::UnitTestImpl* const impl = internal::GetUnitTestImpl();
  const TestCase* const test_case = impl->current_test_case();
  const TestInfo* const first_test_info = test_case->test_info_list()[0];
  const internal::TypeId first_fixture_id = first_test_info->fixture_class_id_;
  const char* const first_test_name = first_test_info->name();
  const TestInfo* const this_test_info = impl->current_test_info();
  const internal::TypeId this_fixture_id = this_test_info->fixture_class_id_;
  const char* const this_test_name = this_test_info->name();
  if (this_fixture_id != first_fixture_id) {
    const bool first_is_TEST = first_fixture_id == internal::GetTestTypeId();
    const bool this_is_TEST = this_fixture_id == internal::GetTestTypeId();
    if (first_is_TEST || this_is_TEST) {
      const char* const TEST_name =
          first_is_TEST ? first_test_name : this_test_name;
      const char* const TEST_F_name =
          first_is_TEST ? this_test_name : first_test_name;
      ADD_FAILURE()
          << "All tests in the same test case must use the same test fixture\n"
          << "class, so mixing TEST_F and TEST in the same test case is\n"
          << "illegal.  In test case " << this_test_info->test_case_name()
          << ",\n"
          << "test " << TEST_F_name << " is defined using TEST_F but\n"
          << "test " << TEST_name << " is defined using TEST.  You probably\n"
          << "want to change the TEST to TEST_F or move it to another test\n"
          << "case.";
    } else {
      ADD_FAILURE()
          << "All tests in the same test case must use the same test fixture\n"
          << "class.  However, in test case "
          << this_test_info->test_case_name() << ",\n"
          << "you defined test " << first_test_name
          << " and test " << this_test_name << "\n"
          << "using two different test fixture classes.  This can happen if\n"
          << "the two classes are from different namespaces or translation\n"
          << "units and have the same name.  You should probably rename one\n"
          << "of the classes to put the tests into different test cases.";
    }
    return false;
  }
  return true;
}
#if GTEST_HAS_SEH
static std::string* FormatSehExceptionMessage(DWORD exception_code,
                                              const char* location) {
  Message message;
  message << "SEH exception with code 0x" << std::setbase(16) <<
    exception_code << std::setbase(10) << " thrown in " << location << ".";
  return new std::string(message.GetString());
}
#endif
namespace internal {
#if GTEST_HAS_EXCEPTIONS
static std::string FormatCxxExceptionMessage(const char* description,
                                             const char* location) {
  Message message;
  if (description != NULL) {
    message << "C++ exception with description \"" << description << "\"";
  } else {
    message << "Unknown C++ exception";
  }
  message << " thrown in " << location << ".";
  return message.GetString();
}
static std::string PrintTestPartResultToString(
    const TestPartResult& test_part_result);
GoogleTestFailureException::GoogleTestFailureException(
    const TestPartResult& failure)
    : ::std::runtime_error(PrintTestPartResultToString(failure).c_str()) {}
#endif
template <class T, typename Result>
Result HandleSehExceptionsInMethodIfSupported(
    T* object, Result (T::*method)(), const char* location) {
#if GTEST_HAS_SEH
  __try {
    return (object->*method)();
  } __except (internal::UnitTestOptions::GTestShouldProcessSEH(
      GetExceptionCode())) {
    std::string* exception_message = FormatSehExceptionMessage(
        GetExceptionCode(), location);
    internal::ReportFailureInUnknownLocation(TestPartResult::kFatalFailure,
                                             *exception_message);
    delete exception_message;
    return static_cast<Result>(0);
  }
#else
  (void)location;
  return (object->*method)();
#endif
}
template <class T, typename Result>
Result HandleExceptionsInMethodIfSupported(
    T* object, Result (T::*method)(), const char* location) {
  if (internal::GetUnitTestImpl()->catch_exceptions()) {
#if GTEST_HAS_EXCEPTIONS
    try {
      return HandleSehExceptionsInMethodIfSupported(object, method, location);
    } catch (const internal::GoogleTestFailureException&) {
      throw;
    } catch (const std::exception& e) {
      internal::ReportFailureInUnknownLocation(
          TestPartResult::kFatalFailure,
          FormatCxxExceptionMessage(e.what(), location));
    } catch (...) {
      internal::ReportFailureInUnknownLocation(
          TestPartResult::kFatalFailure,
          FormatCxxExceptionMessage(NULL, location));
    }
    return static_cast<Result>(0);
#else
    return HandleSehExceptionsInMethodIfSupported(object, method, location);
#endif
  } else {
    return (object->*method)();
  }
}
}
void Test::Run() {
  if (!HasSameFixtureClass()) return;
  internal::UnitTestImpl* const impl = internal::GetUnitTestImpl();
  impl->os_stack_trace_getter()->UponLeavingGTest();
  internal::HandleExceptionsInMethodIfSupported(this, &Test::SetUp, "SetUp()");
  if (!HasFatalFailure()) {
    impl->os_stack_trace_getter()->UponLeavingGTest();
    internal::HandleExceptionsInMethodIfSupported(
        this, &Test::TestBody, "the test body");
  }
  impl->os_stack_trace_getter()->UponLeavingGTest();
  internal::HandleExceptionsInMethodIfSupported(
      this, &Test::TearDown, "TearDown()");
}
bool Test::HasFatalFailure() {
  return internal::GetUnitTestImpl()->current_test_result()->HasFatalFailure();
}
bool Test::HasNonfatalFailure() {
  return internal::GetUnitTestImpl()->current_test_result()->
      HasNonfatalFailure();
}
TestInfo::TestInfo(const std::string& a_test_case_name,
                   const std::string& a_name,
                   const char* a_type_param,
                   const char* a_value_param,
                   internal::TypeId fixture_class_id,
                   internal::TestFactoryBase* factory)
    : test_case_name_(a_test_case_name),
      name_(a_name),
      type_param_(a_type_param ? new std::string(a_type_param) : NULL),
      value_param_(a_value_param ? new std::string(a_value_param) : NULL),
      fixture_class_id_(fixture_class_id),
      should_run_(false),
      is_disabled_(false),
      matches_filter_(false),
      factory_(factory),
      result_() {}
TestInfo::~TestInfo() { delete factory_; }
namespace internal {
TestInfo* MakeAndRegisterTestInfo(
    const char* test_case_name,
    const char* name,
    const char* type_param,
    const char* value_param,
    TypeId fixture_class_id,
    SetUpTestCaseFunc set_up_tc,
    TearDownTestCaseFunc tear_down_tc,
    TestFactoryBase* factory) {
  TestInfo* const test_info =
      new TestInfo(test_case_name, name, type_param, value_param,
                   fixture_class_id, factory);
  GetUnitTestImpl()->AddTestInfo(set_up_tc, tear_down_tc, test_info);
  return test_info;
}
#if GTEST_HAS_PARAM_TEST
void ReportInvalidTestCaseType(const char* test_case_name,
                               const char* file, int line) {
  Message errors;
  errors
      << "Attempted redefinition of test case " << test_case_name << ".\n"
      << "All tests in the same test case must use the same test fixture\n"
      << "class.  However, in test case " << test_case_name << ", you tried\n"
      << "to define a test using a fixture class different from the one\n"
      << "used earlier. This can happen if the two fixture classes are\n"
      << "from different namespaces and have the same name. You should\n"
      << "probably rename one of the classes to put the tests into different\n"
      << "test cases.";
  fprintf(stderr, "%s %s", FormatFileLocation(file, line).c_str(),
          errors.GetString().c_str());
}
#endif
}
namespace {
class TestNameIs {
 public:
  explicit TestNameIs(const char* name)
      : name_(name) {}
  bool operator()(const TestInfo * test_info) const {
    return test_info && test_info->name() == name_;
  }
 private:
  std::string name_;
};
}
namespace internal {
void UnitTestImpl::RegisterParameterizedTests() {
#if GTEST_HAS_PARAM_TEST
  if (!parameterized_tests_registered_) {
    parameterized_test_registry_.RegisterTests();
    parameterized_tests_registered_ = true;
  }
#endif
}
}
void TestInfo::Run() {
  if (!should_run_) return;
  internal::UnitTestImpl* const impl = internal::GetUnitTestImpl();
  impl->set_current_test_info(this);
  TestEventListener* repeater = UnitTest::GetInstance()->listeners().repeater();
  repeater->OnTestStart(*this);
  const TimeInMillis start = internal::GetTimeInMillis();
  impl->os_stack_trace_getter()->UponLeavingGTest();
  Test* const test = internal::HandleExceptionsInMethodIfSupported(
      factory_, &internal::TestFactoryBase::CreateTest,
      "the test fixture's constructor");
  if ((test != NULL) && !Test::HasFatalFailure()) {
    test->Run();
  }
  impl->os_stack_trace_getter()->UponLeavingGTest();
  internal::HandleExceptionsInMethodIfSupported(
      test, &Test::DeleteSelf_, "the test fixture's destructor");
  result_.set_elapsed_time(internal::GetTimeInMillis() - start);
  repeater->OnTestEnd(*this);
  impl->set_current_test_info(NULL);
}
int TestCase::successful_test_count() const {
  return CountIf(test_info_list_, TestPassed);
}
int TestCase::failed_test_count() const {
  return CountIf(test_info_list_, TestFailed);
}
int TestCase::reportable_disabled_test_count() const {
  return CountIf(test_info_list_, TestReportableDisabled);
}
int TestCase::disabled_test_count() const {
  return CountIf(test_info_list_, TestDisabled);
}
int TestCase::reportable_test_count() const {
  return CountIf(test_info_list_, TestReportable);
}
int TestCase::test_to_run_count() const {
  return CountIf(test_info_list_, ShouldRunTest);
}
int TestCase::total_test_count() const {
  return static_cast<int>(test_info_list_.size());
}
TestCase::TestCase(const char* a_name, const char* a_type_param,
                   Test::SetUpTestCaseFunc set_up_tc,
                   Test::TearDownTestCaseFunc tear_down_tc)
    : name_(a_name),
      type_param_(a_type_param ? new std::string(a_type_param) : NULL),
      set_up_tc_(set_up_tc),
      tear_down_tc_(tear_down_tc),
      should_run_(false),
      elapsed_time_(0) {
}
TestCase::~TestCase() {
  ForEach(test_info_list_, internal::Delete<TestInfo>);
}
const TestInfo* TestCase::GetTestInfo(int i) const {
  const int index = GetElementOr(test_indices_, i, -1);
  return index < 0 ? NULL : test_info_list_[index];
}
TestInfo* TestCase::GetMutableTestInfo(int i) {
  const int index = GetElementOr(test_indices_, i, -1);
  return index < 0 ? NULL : test_info_list_[index];
}
void TestCase::AddTestInfo(TestInfo * test_info) {
  test_info_list_.push_back(test_info);
  test_indices_.push_back(static_cast<int>(test_indices_.size()));
}
void TestCase::Run() {
  if (!should_run_) return;
  internal::UnitTestImpl* const impl = internal::GetUnitTestImpl();
  impl->set_current_test_case(this);
  TestEventListener* repeater = UnitTest::GetInstance()->listeners().repeater();
  repeater->OnTestCaseStart(*this);
  impl->os_stack_trace_getter()->UponLeavingGTest();
  internal::HandleExceptionsInMethodIfSupported(
      this, &TestCase::RunSetUpTestCase, "SetUpTestCase()");
  const internal::TimeInMillis start = internal::GetTimeInMillis();
  for (int i = 0; i < total_test_count(); i++) {
    GetMutableTestInfo(i)->Run();
  }
  elapsed_time_ = internal::GetTimeInMillis() - start;
  impl->os_stack_trace_getter()->UponLeavingGTest();
  internal::HandleExceptionsInMethodIfSupported(
      this, &TestCase::RunTearDownTestCase, "TearDownTestCase()");
  repeater->OnTestCaseEnd(*this);
  impl->set_current_test_case(NULL);
}
void TestCase::ClearResult() {
  ad_hoc_test_result_.Clear();
  ForEach(test_info_list_, TestInfo::ClearTestResult);
}
void TestCase::ShuffleTests(internal::Random* random) {
  Shuffle(random, &test_indices_);
}
void TestCase::UnshuffleTests() {
  for (size_t i = 0; i < test_indices_.size(); i++) {
    test_indices_[i] = static_cast<int>(i);
  }
}
static std::string FormatCountableNoun(int count,
                                       const char * singular_form,
                                       const char * plural_form) {
  return internal::StreamableToString(count) + " " +
      (count == 1 ? singular_form : plural_form);
}
static std::string FormatTestCount(int test_count) {
  return FormatCountableNoun(test_count, "test", "tests");
}
static std::string FormatTestCaseCount(int test_case_count) {
  return FormatCountableNoun(test_case_count, "test case", "test cases");
}
static const char * TestPartResultTypeToString(TestPartResult::Type type) {
  switch (type) {
    case TestPartResult::kSuccess:
      return "Success";
    case TestPartResult::kNonFatalFailure:
    case TestPartResult::kFatalFailure:
#ifdef _MSC_VER
      return "error: ";
#else
      return "Failure\n";
#endif
    default:
      return "Unknown result type";
  }
}
namespace internal {
static std::string PrintTestPartResultToString(
    const TestPartResult& test_part_result) {
  return (Message()
          << internal::FormatFileLocation(test_part_result.file_name(),
                                          test_part_result.line_number())
          << " " << TestPartResultTypeToString(test_part_result.type())
          << test_part_result.message()).GetString();
}
static void PrintTestPartResult(const TestPartResult& test_part_result) {
  const std::string& result =
      PrintTestPartResultToString(test_part_result);
  printf("%s\n", result.c_str());
  fflush(stdout);
#if GTEST_OS_WINDOWS && !GTEST_OS_WINDOWS_MOBILE
  ::OutputDebugStringA(result.c_str());
  ::OutputDebugStringA("\n");
#endif
}
enum GTestColor {
  COLOR_DEFAULT,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW
};
#if GTEST_OS_WINDOWS && !GTEST_OS_WINDOWS_MOBILE
WORD GetColorAttribute(GTestColor color) {
  switch (color) {
    case COLOR_RED: return FOREGROUND_RED;
    case COLOR_GREEN: return FOREGROUND_GREEN;
    case COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
    default: return 0;
  }
}
#else
const char* GetAnsiColorCode(GTestColor color) {
  switch (color) {
    case COLOR_RED: return "1";
    case COLOR_GREEN: return "2";
    case COLOR_YELLOW: return "3";
    default: return NULL;
  };
}
#endif
bool ShouldUseColor(bool stdout_is_tty) {
  const char* const gtest_color = GTEST_FLAG(color).c_str();
  if (String::CaseInsensitiveCStringEquals(gtest_color, "auto")) {
#if GTEST_OS_WINDOWS
    return stdout_is_tty;
#else
    const char* const term = posix::GetEnv("TERM");
    const bool term_supports_color =
        String::CStringEquals(term, "xterm") ||
        String::CStringEquals(term, "xterm-color") ||
        String::CStringEquals(term, "xterm-256color") ||
        String::CStringEquals(term, "screen") ||
        String::CStringEquals(term, "screen-256color") ||
        String::CStringEquals(term, "linux") ||
        String::CStringEquals(term, "cygwin");
    return stdout_is_tty && term_supports_color;
#endif
  }
  return String::CaseInsensitiveCStringEquals(gtest_color, "yes") ||
      String::CaseInsensitiveCStringEquals(gtest_color, "true") ||
      String::CaseInsensitiveCStringEquals(gtest_color, "t") ||
      String::CStringEquals(gtest_color, "1");
}
void ColoredPrintf(GTestColor color, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
#if GTEST_OS_WINDOWS_MOBILE || GTEST_OS_SYMBIAN || GTEST_OS_ZOS || GTEST_OS_IOS
  const bool use_color = false;
#else
  static const bool in_color_mode =
      ShouldUseColor(posix::IsATTY(posix::FileNo(stdout)) != 0);
  const bool use_color = in_color_mode && (color != COLOR_DEFAULT);
#endif
  if (!use_color) {
    vprintf(fmt, args);
    va_end(args);
    return;
  }
#if GTEST_OS_WINDOWS && !GTEST_OS_WINDOWS_MOBILE
  const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
  GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
  const WORD old_color_attrs = buffer_info.wAttributes;
  fflush(stdout);
  SetConsoleTextAttribute(stdout_handle,
                          GetColorAttribute(color) | FOREGROUND_INTENSITY);
  vprintf(fmt, args);
  fflush(stdout);
  SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
  printf("\033[0;3%sm", GetAnsiColorCode(color));
  vprintf(fmt, args);
  printf("\033[m");
#endif
  va_end(args);
}
static const char kTypeParamLabel[] = "TypeParam";
static const char kValueParamLabel[] = "GetParam()";
void PrintFullTestCommentIfPresent(const TestInfo& test_info) {
  const char* const type_param = test_info.type_param();
  const char* const value_param = test_info.value_param();
  if (type_param != NULL || value_param != NULL) {
    printf(", where ");
    if (type_param != NULL) {
      printf("%s = %s", kTypeParamLabel, type_param);
      if (value_param != NULL)
        printf(" and ");
    }
    if (value_param != NULL) {
      printf("%s = %s", kValueParamLabel, value_param);
    }
  }
}
class PrettyUnitTestResultPrinter : public TestEventListener {
 public:
  PrettyUnitTestResultPrinter() {}
  static void PrintTestName(const char * test_case, const char * test) {
    printf("%s.%s", test_case, test);
  }
  virtual void OnTestProgramStart(const UnitTest& ) {}
  virtual void OnTestIterationStart(const UnitTest& unit_test, int iteration);
  virtual void OnEnvironmentsSetUpStart(const UnitTest& unit_test);
  virtual void OnEnvironmentsSetUpEnd(const UnitTest& ) {}
  virtual void OnTestCaseStart(const TestCase& test_case);
  virtual void OnTestStart(const TestInfo& test_info);
  virtual void OnTestPartResult(const TestPartResult& result);
  virtual void OnTestEnd(const TestInfo& test_info);
  virtual void OnTestCaseEnd(const TestCase& test_case);
  virtual void OnEnvironmentsTearDownStart(const UnitTest& unit_test);
  virtual void OnEnvironmentsTearDownEnd(const UnitTest& ) {}
  virtual void OnTestIterationEnd(const UnitTest& unit_test, int iteration);
  virtual void OnTestProgramEnd(const UnitTest& ) {}
 private:
  static void PrintFailedTests(const UnitTest& unit_test);
};
void PrettyUnitTestResultPrinter::OnTestIterationStart(
    const UnitTest& unit_test, int iteration) {
  if (GTEST_FLAG(repeat) != 1)
    printf("\nRepeating all tests (iteration %d) . . .\n\n", iteration + 1);
  const char* const filter = GTEST_FLAG(filter).c_str();
  if (!String::CStringEquals(filter, kUniversalFilter)) {
    ColoredPrintf(COLOR_YELLOW,
                  "Note: %s filter = %s\n", GTEST_NAME_, filter);
  }
  if (internal::ShouldShard(kTestTotalShards, kTestShardIndex, false)) {
    const Int32 shard_index = Int32FromEnvOrDie(kTestShardIndex, -1);
    ColoredPrintf(COLOR_YELLOW,
                  "Note: This is test shard %d of %s.\n",
                  static_cast<int>(shard_index) + 1,
                  internal::posix::GetEnv(kTestTotalShards));
  }
  if (GTEST_FLAG(shuffle)) {
    ColoredPrintf(COLOR_YELLOW,
                  "Note: Randomizing tests' orders with a seed of %d .\n",
                  unit_test.random_seed());
  }
  ColoredPrintf(COLOR_GREEN, "[==========] ");
  printf("Running %s from %s.\n",
         FormatTestCount(unit_test.test_to_run_count()).c_str(),
         FormatTestCaseCount(unit_test.test_case_to_run_count()).c_str());
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnEnvironmentsSetUpStart(
    const UnitTest& ) {
  ColoredPrintf(COLOR_GREEN, "[----------] ");
  printf("Global test environment set-up.\n");
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnTestCaseStart(const TestCase& test_case) {
  const std::string counts =
      FormatCountableNoun(test_case.test_to_run_count(), "test", "tests");
  ColoredPrintf(COLOR_GREEN, "[----------] ");
  printf("%s from %s", counts.c_str(), test_case.name());
  if (test_case.type_param() == NULL) {
    printf("\n");
  } else {
    printf(", where %s = %s\n", kTypeParamLabel, test_case.type_param());
  }
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnTestStart(const TestInfo& test_info) {
  ColoredPrintf(COLOR_GREEN, "[ RUN      ] ");
  PrintTestName(test_info.test_case_name(), test_info.name());
  printf("\n");
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnTestPartResult(
    const TestPartResult& result) {
  if (result.type() == TestPartResult::kSuccess)
    return;
  PrintTestPartResult(result);
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnTestEnd(const TestInfo& test_info) {
  if (test_info.result()->Passed()) {
    ColoredPrintf(COLOR_GREEN, "[       OK ] ");
  } else {
    ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
  }
  PrintTestName(test_info.test_case_name(), test_info.name());
  if (test_info.result()->Failed())
    PrintFullTestCommentIfPresent(test_info);
  if (GTEST_FLAG(print_time)) {
    printf(" (%s ms)\n", internal::StreamableToString(
           test_info.result()->elapsed_time()).c_str());
  } else {
    printf("\n");
  }
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnTestCaseEnd(const TestCase& test_case) {
  if (!GTEST_FLAG(print_time)) return;
  const std::string counts =
      FormatCountableNoun(test_case.test_to_run_count(), "test", "tests");
  ColoredPrintf(COLOR_GREEN, "[----------] ");
  printf("%s from %s (%s ms total)\n\n",
         counts.c_str(), test_case.name(),
         internal::StreamableToString(test_case.elapsed_time()).c_str());
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::OnEnvironmentsTearDownStart(
    const UnitTest& ) {
  ColoredPrintf(COLOR_GREEN, "[----------] ");
  printf("Global test environment tear-down\n");
  fflush(stdout);
}
void PrettyUnitTestResultPrinter::PrintFailedTests(const UnitTest& unit_test) {
  const int failed_test_count = unit_test.failed_test_count();
  if (failed_test_count == 0) {
    return;
  }
  for (int i = 0; i < unit_test.total_test_case_count(); ++i) {
    const TestCase& test_case = *unit_test.GetTestCase(i);
    if (!test_case.should_run() || (test_case.failed_test_count() == 0)) {
      continue;
    }
    for (int j = 0; j < test_case.total_test_count(); ++j) {
      const TestInfo& test_info = *test_case.GetTestInfo(j);
      if (!test_info.should_run() || test_info.result()->Passed()) {
        continue;
      }
      ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
      printf("%s.%s", test_case.name(), test_info.name());
      PrintFullTestCommentIfPresent(test_info);
      printf("\n");
    }
  }
}
void PrettyUnitTestResultPrinter::OnTestIterationEnd(const UnitTest& unit_test,
                                                     int ) {
  ColoredPrintf(COLOR_GREEN, "[==========] ");
  printf("%s from %s ran.",
         FormatTestCount(unit_test.test_to_run_count()).c_str(),
         FormatTestCaseCount(unit_test.test_case_to_run_count()).c_str());
  if (GTEST_FLAG(print_time)) {
    printf(" (%s ms total)",
           internal::StreamableToString(unit_test.elapsed_time()).c_str());
  }
  printf("\n");
  ColoredPrintf(COLOR_GREEN, "[  PASSED  ] ");
  printf("%s.\n", FormatTestCount(unit_test.successful_test_count()).c_str());
  int num_failures = unit_test.failed_test_count();
  if (!unit_test.Passed()) {
    const int failed_test_count = unit_test.failed_test_count();
    ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
    printf("%s, listed below:\n", FormatTestCount(failed_test_count).c_str());
    PrintFailedTests(unit_test);
    printf("\n%2d FAILED %s\n", num_failures,
                        num_failures == 1 ? "TEST" : "TESTS");
  }
  int num_disabled = unit_test.reportable_disabled_test_count();
  if (num_disabled && !GTEST_FLAG(also_run_disabled_tests)) {
    if (!num_failures) {
      printf("\n");
    }
    ColoredPrintf(COLOR_YELLOW,
                  "  YOU HAVE %d DISABLED %s\n\n",
                  num_disabled,
                  num_disabled == 1 ? "TEST" : "TESTS");
  }
  fflush(stdout);
}
class TestEventRepeater : public TestEventListener {
 public:
  TestEventRepeater() : forwarding_enabled_(true) {}
  virtual ~TestEventRepeater();
  void Append(TestEventListener *listener);
  TestEventListener* Release(TestEventListener* listener);
  bool forwarding_enabled() const { return forwarding_enabled_; }
  void set_forwarding_enabled(bool enable) { forwarding_enabled_ = enable; }
  virtual void OnTestProgramStart(const UnitTest& unit_test);
  virtual void OnTestIterationStart(const UnitTest& unit_test, int iteration);
  virtual void OnEnvironmentsSetUpStart(const UnitTest& unit_test);
  virtual void OnEnvironmentsSetUpEnd(const UnitTest& unit_test);
  virtual void OnTestCaseStart(const TestCase& test_case);
  virtual void OnTestStart(const TestInfo& test_info);
  virtual void OnTestPartResult(const TestPartResult& result);
  virtual void OnTestEnd(const TestInfo& test_info);
  virtual void OnTestCaseEnd(const TestCase& test_case);
  virtual void OnEnvironmentsTearDownStart(const UnitTest& unit_test);
  virtual void OnEnvironmentsTearDownEnd(const UnitTest& unit_test);
  virtual void OnTestIterationEnd(const UnitTest& unit_test, int iteration);
  virtual void OnTestProgramEnd(const UnitTest& unit_test);
 private:
  bool forwarding_enabled_;
  std::vector<TestEventListener*> listeners_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestEventRepeater);
};
TestEventRepeater::~TestEventRepeater() {
  ForEach(listeners_, Delete<TestEventListener>);
}
void TestEventRepeater::Append(TestEventListener *listener) {
  listeners_.push_back(listener);
}
TestEventListener* TestEventRepeater::Release(TestEventListener *listener) {
  for (size_t i = 0; i < listeners_.size(); ++i) {
    if (listeners_[i] == listener) {
      listeners_.erase(listeners_.begin() + i);
      return listener;
    }
  }
  return NULL;
}
#define GTEST_REPEATER_METHOD_(Name,Type) \
void TestEventRepeater::Name(const Type& parameter) { \
  if (forwarding_enabled_) { \
    for (size_t i = 0; i < listeners_.size(); i++) { \
      listeners_[i]->Name(parameter); \
    } \
  } \
}
#define GTEST_REVERSE_REPEATER_METHOD_(Name,Type) \
void TestEventRepeater::Name(const Type& parameter) { \
  if (forwarding_enabled_) { \
    for (int i = static_cast<int>(listeners_.size()) - 1; i >= 0; i--) { \
      listeners_[i]->Name(parameter); \
    } \
  } \
}
GTEST_REPEATER_METHOD_(OnTestProgramStart, UnitTest)
GTEST_REPEATER_METHOD_(OnEnvironmentsSetUpStart, UnitTest)
GTEST_REPEATER_METHOD_(OnTestCaseStart, TestCase)
GTEST_REPEATER_METHOD_(OnTestStart, TestInfo)
GTEST_REPEATER_METHOD_(OnTestPartResult, TestPartResult)
GTEST_REPEATER_METHOD_(OnEnvironmentsTearDownStart, UnitTest)
GTEST_REVERSE_REPEATER_METHOD_(OnEnvironmentsSetUpEnd, UnitTest)
GTEST_REVERSE_REPEATER_METHOD_(OnEnvironmentsTearDownEnd, UnitTest)
GTEST_REVERSE_REPEATER_METHOD_(OnTestEnd, TestInfo)
GTEST_REVERSE_REPEATER_METHOD_(OnTestCaseEnd, TestCase)
GTEST_REVERSE_REPEATER_METHOD_(OnTestProgramEnd, UnitTest)
#undef GTEST_REPEATER_METHOD_
#undef GTEST_REVERSE_REPEATER_METHOD_
void TestEventRepeater::OnTestIterationStart(const UnitTest& unit_test,
                                             int iteration) {
  if (forwarding_enabled_) {
    for (size_t i = 0; i < listeners_.size(); i++) {
      listeners_[i]->OnTestIterationStart(unit_test, iteration);
    }
  }
}
void TestEventRepeater::OnTestIterationEnd(const UnitTest& unit_test,
                                           int iteration) {
  if (forwarding_enabled_) {
    for (int i = static_cast<int>(listeners_.size()) - 1; i >= 0; i--) {
      listeners_[i]->OnTestIterationEnd(unit_test, iteration);
    }
  }
}
class XmlUnitTestResultPrinter : public EmptyTestEventListener {
 public:
  explicit XmlUnitTestResultPrinter(const char* output_file);
  virtual void OnTestIterationEnd(const UnitTest& unit_test, int iteration);
 private:
  static bool IsNormalizableWhitespace(char c) {
    return c == 0x9 || c == 0xA || c == 0xD;
  }
  static bool IsValidXmlCharacter(char c) {
    return IsNormalizableWhitespace(c) || c >= 0x20;
  }
  static std::string EscapeXml(const std::string& str, bool is_attribute);
  static std::string RemoveInvalidXmlCharacters(const std::string& str);
  static std::string EscapeXmlAttribute(const std::string& str) {
    return EscapeXml(str, true);
  }
  static std::string EscapeXmlText(const char* str) {
    return EscapeXml(str, false);
  }
  static void OutputXmlAttribute(std::ostream* stream,
                                 const std::string& element_name,
                                 const std::string& name,
                                 const std::string& value);
  static void OutputXmlCDataSection(::std::ostream* stream, const char* data);
  static void OutputXmlTestInfo(::std::ostream* stream,
                                const char* test_case_name,
                                const TestInfo& test_info);
  static void PrintXmlTestCase(::std::ostream* stream,
                               const TestCase& test_case);
  static void PrintXmlUnitTest(::std::ostream* stream,
                               const UnitTest& unit_test);
  static std::string TestPropertiesAsXmlAttributes(const TestResult& result);
  const std::string output_file_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(XmlUnitTestResultPrinter);
};
XmlUnitTestResultPrinter::XmlUnitTestResultPrinter(const char* output_file)
    : output_file_(output_file) {
  if (output_file_.c_str() == NULL || output_file_.empty()) {
    fprintf(stderr, "XML output file may not be null\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
}
void XmlUnitTestResultPrinter::OnTestIterationEnd(const UnitTest& unit_test,
                                                  int ) {
  FILE* xmlout = NULL;
  FilePath output_file(output_file_);
  FilePath output_dir(output_file.RemoveFileName());
  if (output_dir.CreateDirectoriesRecursively()) {
    xmlout = posix::FOpen(output_file_.c_str(), "w");
  }
  if (xmlout == NULL) {
    fprintf(stderr,
            "Unable to open file \"%s\"\n",
            output_file_.c_str());
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
  std::stringstream stream;
  PrintXmlUnitTest(&stream, unit_test);
  fprintf(xmlout, "%s", StringStreamToString(&stream).c_str());
  fclose(xmlout);
}
std::string XmlUnitTestResultPrinter::EscapeXml(
    const std::string& str, bool is_attribute) {
  Message m;
  for (size_t i = 0; i < str.size(); ++i) {
    const char ch = str[i];
    switch (ch) {
      case '<':
        m << "&lt;";
        break;
      case '>':
        m << "&gt;";
        break;
      case '&':
        m << "&amp;";
        break;
      case '\'':
        if (is_attribute)
          m << "&apos;";
        else
          m << '\'';
        break;
      case '"':
        if (is_attribute)
          m << "&quot;";
        else
          m << '"';
        break;
      default:
        if (IsValidXmlCharacter(ch)) {
          if (is_attribute && IsNormalizableWhitespace(ch))
            m << "&#x" << String::FormatByte(static_cast<unsigned char>(ch))
              << ";";
          else
            m << ch;
        }
        break;
    }
  }
  return m.GetString();
}
std::string XmlUnitTestResultPrinter::RemoveInvalidXmlCharacters(
    const std::string& str) {
  std::string output;
  output.reserve(str.size());
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    if (IsValidXmlCharacter(*it))
      output.push_back(*it);
  return output;
}
std::string FormatTimeInMillisAsSeconds(TimeInMillis ms) {
  ::std::stringstream ss;
  ss << ms/1000.0;
  return ss.str();
}
std::string FormatEpochTimeInMillisAsIso8601(TimeInMillis ms) {
  time_t seconds = static_cast<time_t>(ms / 1000);
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
  const struct tm* const time_struct = localtime(&seconds);
#pragma warning(pop)
#else
  const struct tm* const time_struct = localtime(&seconds);
#endif
  if (time_struct == NULL)
    return "";
  return StreamableToString(time_struct->tm_year + 1900) + "-" +
      String::FormatIntWidth2(time_struct->tm_mon + 1) + "-" +
      String::FormatIntWidth2(time_struct->tm_mday) + "T" +
      String::FormatIntWidth2(time_struct->tm_hour) + ":" +
      String::FormatIntWidth2(time_struct->tm_min) + ":" +
      String::FormatIntWidth2(time_struct->tm_sec);
}
void XmlUnitTestResultPrinter::OutputXmlCDataSection(::std::ostream* stream,
                                                     const char* data) {
  const char* segment = data;
  *stream << "<![CDATA[";
  for (;;) {
    const char* const next_segment = strstr(segment, "]]>");
    if (next_segment != NULL) {
      stream->write(
          segment, static_cast<std::streamsize>(next_segment - segment));
      *stream << "]]>]]&gt;<![CDATA[";
      segment = next_segment + strlen("]]>");
    } else {
      *stream << segment;
      break;
    }
  }
  *stream << "]]>";
}
void XmlUnitTestResultPrinter::OutputXmlAttribute(
    std::ostream* stream,
    const std::string& element_name,
    const std::string& name,
    const std::string& value) {
  const std::vector<std::string>& allowed_names =
      GetReservedAttributesForElement(element_name);
  GTEST_CHECK_(std::find(allowed_names.begin(), allowed_names.end(), name) !=
                   allowed_names.end())
      << "Attribute " << name << " is not allowed for element <" << element_name
      << ">.";
  *stream << " " << name << "=\"" << EscapeXmlAttribute(value) << "\"";
}
void XmlUnitTestResultPrinter::OutputXmlTestInfo(::std::ostream* stream,
                                                 const char* test_case_name,
                                                 const TestInfo& test_info) {
  const TestResult& result = *test_info.result();
  const std::string kTestcase = "testcase";
  *stream << "    <testcase";
  OutputXmlAttribute(stream, kTestcase, "name", test_info.name());
  if (test_info.value_param() != NULL) {
    OutputXmlAttribute(stream, kTestcase, "value_param",
                       test_info.value_param());
  }
  if (test_info.type_param() != NULL) {
    OutputXmlAttribute(stream, kTestcase, "type_param", test_info.type_param());
  }
  OutputXmlAttribute(stream, kTestcase, "status",
                     test_info.should_run() ? "run" : "notrun");
  OutputXmlAttribute(stream, kTestcase, "time",
                     FormatTimeInMillisAsSeconds(result.elapsed_time()));
  OutputXmlAttribute(stream, kTestcase, "classname", test_case_name);
  *stream << TestPropertiesAsXmlAttributes(result);
  int failures = 0;
  for (int i = 0; i < result.total_part_count(); ++i) {
    const TestPartResult& part = result.GetTestPartResult(i);
    if (part.failed()) {
      if (++failures == 1) {
        *stream << ">\n";
      }
      const string location = internal::FormatCompilerIndependentFileLocation(
          part.file_name(), part.line_number());
      const string summary = location + "\n" + part.summary();
      *stream << "      <failure message=\""
              << EscapeXmlAttribute(summary.c_str())
              << "\" type=\"\">";
      const string detail = location + "\n" + part.message();
      OutputXmlCDataSection(stream, RemoveInvalidXmlCharacters(detail).c_str());
      *stream << "</failure>\n";
    }
  }
  if (failures == 0)
    *stream << " />\n";
  else
    *stream << "    </testcase>\n";
}
void XmlUnitTestResultPrinter::PrintXmlTestCase(std::ostream* stream,
                                                const TestCase& test_case) {
  const std::string kTestsuite = "testsuite";
  *stream << "  <" << kTestsuite;
  OutputXmlAttribute(stream, kTestsuite, "name", test_case.name());
  OutputXmlAttribute(stream, kTestsuite, "tests",
                     StreamableToString(test_case.reportable_test_count()));
  OutputXmlAttribute(stream, kTestsuite, "failures",
                     StreamableToString(test_case.failed_test_count()));
  OutputXmlAttribute(
      stream, kTestsuite, "disabled",
      StreamableToString(test_case.reportable_disabled_test_count()));
  OutputXmlAttribute(stream, kTestsuite, "errors", "0");
  OutputXmlAttribute(stream, kTestsuite, "time",
                     FormatTimeInMillisAsSeconds(test_case.elapsed_time()));
  *stream << TestPropertiesAsXmlAttributes(test_case.ad_hoc_test_result())
          << ">\n";
  for (int i = 0; i < test_case.total_test_count(); ++i) {
    if (test_case.GetTestInfo(i)->is_reportable())
      OutputXmlTestInfo(stream, test_case.name(), *test_case.GetTestInfo(i));
  }
  *stream << "  </" << kTestsuite << ">\n";
}
void XmlUnitTestResultPrinter::PrintXmlUnitTest(std::ostream* stream,
                                                const UnitTest& unit_test) {
  const std::string kTestsuites = "testsuites";
  *stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  *stream << "<" << kTestsuites;
  OutputXmlAttribute(stream, kTestsuites, "tests",
                     StreamableToString(unit_test.reportable_test_count()));
  OutputXmlAttribute(stream, kTestsuites, "failures",
                     StreamableToString(unit_test.failed_test_count()));
  OutputXmlAttribute(
      stream, kTestsuites, "disabled",
      StreamableToString(unit_test.reportable_disabled_test_count()));
  OutputXmlAttribute(stream, kTestsuites, "errors", "0");
  OutputXmlAttribute(
      stream, kTestsuites, "timestamp",
      FormatEpochTimeInMillisAsIso8601(unit_test.start_timestamp()));
  OutputXmlAttribute(stream, kTestsuites, "time",
                     FormatTimeInMillisAsSeconds(unit_test.elapsed_time()));
  if (GTEST_FLAG(shuffle)) {
    OutputXmlAttribute(stream, kTestsuites, "random_seed",
                       StreamableToString(unit_test.random_seed()));
  }
  *stream << TestPropertiesAsXmlAttributes(unit_test.ad_hoc_test_result());
  OutputXmlAttribute(stream, kTestsuites, "name", "AllTests");
  *stream << ">\n";
  for (int i = 0; i < unit_test.total_test_case_count(); ++i) {
    if (unit_test.GetTestCase(i)->reportable_test_count() > 0)
      PrintXmlTestCase(stream, *unit_test.GetTestCase(i));
  }
  *stream << "</" << kTestsuites << ">\n";
}
std::string XmlUnitTestResultPrinter::TestPropertiesAsXmlAttributes(
    const TestResult& result) {
  Message attributes;
  for (int i = 0; i < result.test_property_count(); ++i) {
    const TestProperty& property = result.GetTestProperty(i);
    attributes << " " << property.key() << "="
        << "\"" << EscapeXmlAttribute(property.value()) << "\"";
  }
  return attributes.GetString();
}
#if GTEST_CAN_STREAM_RESULTS_
string StreamingListener::UrlEncode(const char* str) {
  string result;
  result.reserve(strlen(str) + 1);
  for (char ch = *str; ch != '\0'; ch = *++str) {
    switch (ch) {
      case '%':
      case '=':
      case '&':
      case '\n':
        result.append("%" + String::FormatByte(static_cast<unsigned char>(ch)));
        break;
      default:
        result.push_back(ch);
        break;
    }
  }
  return result;
}
void StreamingListener::SocketWriter::MakeConnection() {
  GTEST_CHECK_(sockfd_ == -1)
      << "MakeConnection() can't be called when there is already a connection.";
  addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  addrinfo* servinfo = NULL;
  const int error_num = getaddrinfo(
      host_name_.c_str(), port_num_.c_str(), &hints, &servinfo);
  if (error_num != 0) {
    GTEST_LOG_(WARNING) << "stream_result_to: getaddrinfo() failed: "
                        << gai_strerror(error_num);
  }
  for (addrinfo* cur_addr = servinfo; sockfd_ == -1 && cur_addr != NULL;
       cur_addr = cur_addr->ai_next) {
    sockfd_ = socket(
        cur_addr->ai_family, cur_addr->ai_socktype, cur_addr->ai_protocol);
    if (sockfd_ != -1) {
      if (connect(sockfd_, cur_addr->ai_addr, cur_addr->ai_addrlen) == -1) {
        close(sockfd_);
        sockfd_ = -1;
      }
    }
  }
  freeaddrinfo(servinfo);
  if (sockfd_ == -1) {
    GTEST_LOG_(WARNING) << "stream_result_to: failed to connect to "
                        << host_name_ << ":" << port_num_;
  }
}
#endif
ScopedTrace::ScopedTrace(const char* file, int line, const Message& message)
    GTEST_LOCK_EXCLUDED_(&UnitTest::mutex_) {
  TraceInfo trace;
  trace.file = file;
  trace.line = line;
  trace.message = message.GetString();
  UnitTest::GetInstance()->PushGTestTrace(trace);
}
ScopedTrace::~ScopedTrace()
    GTEST_LOCK_EXCLUDED_(&UnitTest::mutex_) {
  UnitTest::GetInstance()->PopGTestTrace();
}
string OsStackTraceGetter::CurrentStackTrace(int ,
                                             int )
    GTEST_LOCK_EXCLUDED_(mutex_) {
  return "";
}
void OsStackTraceGetter::UponLeavingGTest()
    GTEST_LOCK_EXCLUDED_(mutex_) {
}
const char* const
OsStackTraceGetter::kElidedFramesMarker =
    "... " GTEST_NAME_ " internal frames ...";
class ScopedPrematureExitFile {
 public:
  explicit ScopedPrematureExitFile(const char* premature_exit_filepath)
      : premature_exit_filepath_(premature_exit_filepath) {
    if (premature_exit_filepath != NULL && *premature_exit_filepath != '\0') {
      FILE* pfile = posix::FOpen(premature_exit_filepath, "w");
      fwrite("0", 1, 1, pfile);
      fclose(pfile);
    }
  }
  ~ScopedPrematureExitFile() {
    if (premature_exit_filepath_ != NULL && *premature_exit_filepath_ != '\0') {
      remove(premature_exit_filepath_);
    }
  }
 private:
  const char* const premature_exit_filepath_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(ScopedPrematureExitFile);
};
}
TestEventListeners::TestEventListeners()
    : repeater_(new internal::TestEventRepeater()),
      default_result_printer_(NULL),
      default_xml_generator_(NULL) {
}
TestEventListeners::~TestEventListeners() { delete repeater_; }
void TestEventListeners::Append(TestEventListener* listener) {
  repeater_->Append(listener);
}
TestEventListener* TestEventListeners::Release(TestEventListener* listener) {
  if (listener == default_result_printer_)
    default_result_printer_ = NULL;
  else if (listener == default_xml_generator_)
    default_xml_generator_ = NULL;
  return repeater_->Release(listener);
}
TestEventListener* TestEventListeners::repeater() { return repeater_; }
void TestEventListeners::SetDefaultResultPrinter(TestEventListener* listener) {
  if (default_result_printer_ != listener) {
    delete Release(default_result_printer_);
    default_result_printer_ = listener;
    if (listener != NULL)
      Append(listener);
  }
}
void TestEventListeners::SetDefaultXmlGenerator(TestEventListener* listener) {
  if (default_xml_generator_ != listener) {
    delete Release(default_xml_generator_);
    default_xml_generator_ = listener;
    if (listener != NULL)
      Append(listener);
  }
}
bool TestEventListeners::EventForwardingEnabled() const {
  return repeater_->forwarding_enabled();
}
void TestEventListeners::SuppressEventForwarding() {
  repeater_->set_forwarding_enabled(false);
}
UnitTest* UnitTest::GetInstance() {
#if (_MSC_VER == 1310 && !defined(_DEBUG)) || defined(__BORLANDC__)
  static UnitTest* const instance = new UnitTest;
  return instance;
#else
  static UnitTest instance;
  return &instance;
#endif
}
int UnitTest::successful_test_case_count() const {
  return impl()->successful_test_case_count();
}
int UnitTest::failed_test_case_count() const {
  return impl()->failed_test_case_count();
}
int UnitTest::total_test_case_count() const {
  return impl()->total_test_case_count();
}
int UnitTest::test_case_to_run_count() const {
  return impl()->test_case_to_run_count();
}
int UnitTest::successful_test_count() const {
  return impl()->successful_test_count();
}
int UnitTest::failed_test_count() const { return impl()->failed_test_count(); }
int UnitTest::reportable_disabled_test_count() const {
  return impl()->reportable_disabled_test_count();
}
int UnitTest::disabled_test_count() const {
  return impl()->disabled_test_count();
}
int UnitTest::reportable_test_count() const {
  return impl()->reportable_test_count();
}
int UnitTest::total_test_count() const { return impl()->total_test_count(); }
int UnitTest::test_to_run_count() const { return impl()->test_to_run_count(); }
internal::TimeInMillis UnitTest::start_timestamp() const {
    return impl()->start_timestamp();
}
internal::TimeInMillis UnitTest::elapsed_time() const {
  return impl()->elapsed_time();
}
bool UnitTest::Passed() const { return impl()->Passed(); }
bool UnitTest::Failed() const { return impl()->Failed(); }
const TestCase* UnitTest::GetTestCase(int i) const {
  return impl()->GetTestCase(i);
}
const TestResult& UnitTest::ad_hoc_test_result() const {
  return *impl()->ad_hoc_test_result();
}
TestCase* UnitTest::GetMutableTestCase(int i) {
  return impl()->GetMutableTestCase(i);
}
TestEventListeners& UnitTest::listeners() {
  return *impl()->listeners();
}
Environment* UnitTest::AddEnvironment(Environment* env) {
  if (env == NULL) {
    return NULL;
  }
  impl_->environments().push_back(env);
  return env;
}
void UnitTest::AddTestPartResult(
    TestPartResult::Type result_type,
    const char* file_name,
    int line_number,
    const std::string& message,
    const std::string& os_stack_trace) GTEST_LOCK_EXCLUDED_(mutex_) {
  Message msg;
  msg << message;
  internal::MutexLock lock(&mutex_);
  if (impl_->gtest_trace_stack().size() > 0) {
    msg << "\n" << GTEST_NAME_ << " trace:";
    for (int i = static_cast<int>(impl_->gtest_trace_stack().size());
         i > 0; --i) {
      const internal::TraceInfo& trace = impl_->gtest_trace_stack()[i - 1];
      msg << "\n" << internal::FormatFileLocation(trace.file, trace.line)
          << " " << trace.message;
    }
  }
  if (os_stack_trace.c_str() != NULL && !os_stack_trace.empty()) {
    msg << internal::kStackTraceMarker << os_stack_trace;
  }
  const TestPartResult result =
    TestPartResult(result_type, file_name, line_number,
                   msg.GetString().c_str());
  impl_->GetTestPartResultReporterForCurrentThread()->
      ReportTestPartResult(result);
  if (result_type != TestPartResult::kSuccess) {
    if (GTEST_FLAG(break_on_failure)) {
#if GTEST_OS_WINDOWS
      DebugBreak();
#else
      *static_cast<volatile int*>(NULL) = 1;
#endif
    } else if (GTEST_FLAG(throw_on_failure)) {
#if GTEST_HAS_EXCEPTIONS
      throw internal::GoogleTestFailureException(result);
#else
      exit(1);
#endif
    }
  }
}
void UnitTest::RecordProperty(const std::string& key,
                              const std::string& value) {
  impl_->RecordProperty(TestProperty(key, value));
}
int UnitTest::Run() {
  const bool in_death_test_child_process =
      internal::GTEST_FLAG(internal_run_death_test).length() > 0;
  const internal::ScopedPrematureExitFile premature_exit_file(
      in_death_test_child_process ?
      NULL : internal::posix::GetEnv("TEST_PREMATURE_EXIT_FILE"));
  impl()->set_catch_exceptions(GTEST_FLAG(catch_exceptions));
#if GTEST_HAS_SEH
  if (impl()->catch_exceptions() || in_death_test_child_process) {
# if !GTEST_OS_WINDOWS_MOBILE
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
                 SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
# endif
# if (defined(_MSC_VER) || GTEST_OS_WINDOWS_MINGW) && !GTEST_OS_WINDOWS_MOBILE
    _set_error_mode(_OUT_TO_STDERR);
# endif
# if _MSC_VER >= 1400 && !GTEST_OS_WINDOWS_MOBILE
    if (!GTEST_FLAG(break_on_failure))
      _set_abort_behavior(
          0x0,
          _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
# endif
  }
#endif
  return internal::HandleExceptionsInMethodIfSupported(
      impl(),
      &internal::UnitTestImpl::RunAllTests,
      "auxiliary test code (environments or event listeners)") ? 0 : 1;
}
const char* UnitTest::original_working_dir() const {
  return impl_->original_working_dir_.c_str();
}
const TestCase* UnitTest::current_test_case() const
    GTEST_LOCK_EXCLUDED_(mutex_) {
  internal::MutexLock lock(&mutex_);
  return impl_->current_test_case();
}
const TestInfo* UnitTest::current_test_info() const
    GTEST_LOCK_EXCLUDED_(mutex_) {
  internal::MutexLock lock(&mutex_);
  return impl_->current_test_info();
}
int UnitTest::random_seed() const { return impl_->random_seed(); }
#if GTEST_HAS_PARAM_TEST
internal::ParameterizedTestCaseRegistry&
    UnitTest::parameterized_test_registry()
        GTEST_LOCK_EXCLUDED_(mutex_) {
  return impl_->parameterized_test_registry();
}
#endif
UnitTest::UnitTest() {
  impl_ = new internal::UnitTestImpl(this);
}
UnitTest::~UnitTest() {
  delete impl_;
}
void UnitTest::PushGTestTrace(const internal::TraceInfo& trace)
    GTEST_LOCK_EXCLUDED_(mutex_) {
  internal::MutexLock lock(&mutex_);
  impl_->gtest_trace_stack().push_back(trace);
}
void UnitTest::PopGTestTrace()
    GTEST_LOCK_EXCLUDED_(mutex_) {
  internal::MutexLock lock(&mutex_);
  impl_->gtest_trace_stack().pop_back();
}
namespace internal {
UnitTestImpl::UnitTestImpl(UnitTest* parent)
    : parent_(parent),
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4355)
      default_global_test_part_result_reporter_(this),
      default_per_thread_test_part_result_reporter_(this),
#pragma warning(pop)
#else
      default_global_test_part_result_reporter_(this),
      default_per_thread_test_part_result_reporter_(this),
#endif
      global_test_part_result_repoter_(
          &default_global_test_part_result_reporter_),
      per_thread_test_part_result_reporter_(
          &default_per_thread_test_part_result_reporter_),
#if GTEST_HAS_PARAM_TEST
      parameterized_test_registry_(),
      parameterized_tests_registered_(false),
#endif
      last_death_test_case_(-1),
      current_test_case_(NULL),
      current_test_info_(NULL),
      ad_hoc_test_result_(),
      os_stack_trace_getter_(NULL),
      post_flag_parse_init_performed_(false),
      random_seed_(0),
      random_(0),
      start_timestamp_(0),
      elapsed_time_(0),
#if GTEST_HAS_DEATH_TEST
      death_test_factory_(new DefaultDeathTestFactory),
#endif
      catch_exceptions_(false) {
  listeners()->SetDefaultResultPrinter(new PrettyUnitTestResultPrinter);
}
UnitTestImpl::~UnitTestImpl() {
  ForEach(test_cases_, internal::Delete<TestCase>);
  ForEach(environments_, internal::Delete<Environment>);
  delete os_stack_trace_getter_;
}
void UnitTestImpl::RecordProperty(const TestProperty& test_property) {
  std::string xml_element;
  TestResult* test_result;
  if (current_test_info_ != NULL) {
    xml_element = "testcase";
    test_result = &(current_test_info_->result_);
  } else if (current_test_case_ != NULL) {
    xml_element = "testsuite";
    test_result = &(current_test_case_->ad_hoc_test_result_);
  } else {
    xml_element = "testsuites";
    test_result = &ad_hoc_test_result_;
  }
  test_result->RecordProperty(xml_element, test_property);
}
#if GTEST_HAS_DEATH_TEST
void UnitTestImpl::SuppressTestEventsIfInSubprocess() {
  if (internal_run_death_test_flag_.get() != NULL)
    listeners()->SuppressEventForwarding();
}
#endif
void UnitTestImpl::ConfigureXmlOutput() {
  const std::string& output_format = UnitTestOptions::GetOutputFormat();
  if (output_format == "xml") {
    listeners()->SetDefaultXmlGenerator(new XmlUnitTestResultPrinter(
        UnitTestOptions::GetAbsolutePathToOutputFile().c_str()));
  } else if (output_format != "") {
    printf("WARNING: unrecognized output format \"%s\" ignored.\n",
           output_format.c_str());
    fflush(stdout);
  }
}
#if GTEST_CAN_STREAM_RESULTS_
void UnitTestImpl::ConfigureStreamingOutput() {
  const std::string& target = GTEST_FLAG(stream_result_to);
  if (!target.empty()) {
    const size_t pos = target.find(':');
    if (pos != std::string::npos) {
      listeners()->Append(new StreamingListener(target.substr(0, pos),
                                                target.substr(pos+1)));
    } else {
      printf("WARNING: unrecognized streaming target \"%s\" ignored.\n",
             target.c_str());
      fflush(stdout);
    }
  }
}
#endif
void UnitTestImpl::PostFlagParsingInit() {
  if (!post_flag_parse_init_performed_) {
    post_flag_parse_init_performed_ = true;
#if GTEST_HAS_DEATH_TEST
    InitDeathTestSubprocessControlInfo();
    SuppressTestEventsIfInSubprocess();
#endif
    RegisterParameterizedTests();
    ConfigureXmlOutput();
#if GTEST_CAN_STREAM_RESULTS_
    ConfigureStreamingOutput();
#endif
  }
}
class TestCaseNameIs {
 public:
  explicit TestCaseNameIs(const std::string& name)
      : name_(name) {}
  bool operator()(const TestCase* test_case) const {
    return test_case != NULL && strcmp(test_case->name(), name_.c_str()) == 0;
  }
 private:
  std::string name_;
};
TestCase* UnitTestImpl::GetTestCase(const char* test_case_name,
                                    const char* type_param,
                                    Test::SetUpTestCaseFunc set_up_tc,
                                    Test::TearDownTestCaseFunc tear_down_tc) {
  const std::vector<TestCase*>::const_iterator test_case =
      std::find_if(test_cases_.begin(), test_cases_.end(),
                   TestCaseNameIs(test_case_name));
  if (test_case != test_cases_.end())
    return *test_case;
  TestCase* const new_test_case =
      new TestCase(test_case_name, type_param, set_up_tc, tear_down_tc);
  if (internal::UnitTestOptions::MatchesFilter(test_case_name,
                                               kDeathTestCaseFilter)) {
    ++last_death_test_case_;
    test_cases_.insert(test_cases_.begin() + last_death_test_case_,
                       new_test_case);
  } else {
    test_cases_.push_back(new_test_case);
  }
  test_case_indices_.push_back(static_cast<int>(test_case_indices_.size()));
  return new_test_case;
}
static void SetUpEnvironment(Environment* env) { env->SetUp(); }
static void TearDownEnvironment(Environment* env) { env->TearDown(); }
bool UnitTestImpl::RunAllTests() {
  if (!GTestIsInitialized()) {
    printf("%s",
           "\nThis test program did NOT call ::testing::InitGoogleTest "
           "before calling RUN_ALL_TESTS().  Please fix it.\n");
    return false;
  }
  if (g_help_flag)
    return true;
  PostFlagParsingInit();
  internal::WriteToShardStatusFileIfNeeded();
  bool in_subprocess_for_death_test = false;
#if GTEST_HAS_DEATH_TEST
  in_subprocess_for_death_test = (internal_run_death_test_flag_.get() != NULL);
#endif
  const bool should_shard = ShouldShard(kTestTotalShards, kTestShardIndex,
                                        in_subprocess_for_death_test);
  const bool has_tests_to_run = FilterTests(should_shard
                                              ? HONOR_SHARDING_PROTOCOL
                                              : IGNORE_SHARDING_PROTOCOL) > 0;
  if (GTEST_FLAG(list_tests)) {
    ListTestsMatchingFilter();
    return true;
  }
  random_seed_ = GTEST_FLAG(shuffle) ?
      GetRandomSeedFromFlag(GTEST_FLAG(random_seed)) : 0;
  bool failed = false;
  TestEventListener* repeater = listeners()->repeater();
  start_timestamp_ = GetTimeInMillis();
  repeater->OnTestProgramStart(*parent_);
  const int repeat = in_subprocess_for_death_test ? 1 : GTEST_FLAG(repeat);
  const bool forever = repeat < 0;
  for (int i = 0; forever || i != repeat; i++) {
    ClearNonAdHocTestResult();
    const TimeInMillis start = GetTimeInMillis();
    if (has_tests_to_run && GTEST_FLAG(shuffle)) {
      random()->Reseed(random_seed_);
      ShuffleTests();
    }
    repeater->OnTestIterationStart(*parent_, i);
    if (has_tests_to_run) {
      repeater->OnEnvironmentsSetUpStart(*parent_);
      ForEach(environments_, SetUpEnvironment);
      repeater->OnEnvironmentsSetUpEnd(*parent_);
      if (!Test::HasFatalFailure()) {
        for (int test_index = 0; test_index < total_test_case_count();
             test_index++) {
          GetMutableTestCase(test_index)->Run();
        }
      }
      repeater->OnEnvironmentsTearDownStart(*parent_);
      std::for_each(environments_.rbegin(), environments_.rend(),
                    TearDownEnvironment);
      repeater->OnEnvironmentsTearDownEnd(*parent_);
    }
    elapsed_time_ = GetTimeInMillis() - start;
    repeater->OnTestIterationEnd(*parent_, i);
    if (!Passed()) {
      failed = true;
    }
    UnshuffleTests();
    if (GTEST_FLAG(shuffle)) {
      random_seed_ = GetNextRandomSeed(random_seed_);
    }
  }
  repeater->OnTestProgramEnd(*parent_);
  return !failed;
}
void WriteToShardStatusFileIfNeeded() {
  const char* const test_shard_file = posix::GetEnv(kTestShardStatusFile);
  if (test_shard_file != NULL) {
    FILE* const file = posix::FOpen(test_shard_file, "w");
    if (file == NULL) {
      ColoredPrintf(COLOR_RED,
                    "Could not write to the test shard status file \"%s\" "
                    "specified by the %s environment variable.\n",
                    test_shard_file, kTestShardStatusFile);
      fflush(stdout);
      exit(EXIT_FAILURE);
    }
    fclose(file);
  }
}
bool ShouldShard(const char* total_shards_env,
                 const char* shard_index_env,
                 bool in_subprocess_for_death_test) {
  if (in_subprocess_for_death_test) {
    return false;
  }
  const Int32 total_shards = Int32FromEnvOrDie(total_shards_env, -1);
  const Int32 shard_index = Int32FromEnvOrDie(shard_index_env, -1);
  if (total_shards == -1 && shard_index == -1) {
    return false;
  } else if (total_shards == -1 && shard_index != -1) {
    const Message msg = Message()
      << "Invalid environment variables: you have "
      << kTestShardIndex << " = " << shard_index
      << ", but have left " << kTestTotalShards << " unset.\n";
    ColoredPrintf(COLOR_RED, msg.GetString().c_str());
    fflush(stdout);
    exit(EXIT_FAILURE);
  } else if (total_shards != -1 && shard_index == -1) {
    const Message msg = Message()
      << "Invalid environment variables: you have "
      << kTestTotalShards << " = " << total_shards
      << ", but have left " << kTestShardIndex << " unset.\n";
    ColoredPrintf(COLOR_RED, msg.GetString().c_str());
    fflush(stdout);
    exit(EXIT_FAILURE);
  } else if (shard_index < 0 || shard_index >= total_shards) {
    const Message msg = Message()
      << "Invalid environment variables: we require 0 <= "
      << kTestShardIndex << " < " << kTestTotalShards
      << ", but you have " << kTestShardIndex << "=" << shard_index
      << ", " << kTestTotalShards << "=" << total_shards << ".\n";
    ColoredPrintf(COLOR_RED, msg.GetString().c_str());
    fflush(stdout);
    exit(EXIT_FAILURE);
  }
  return total_shards > 1;
}
Int32 Int32FromEnvOrDie(const char* var, Int32 default_val) {
  const char* str_val = posix::GetEnv(var);
  if (str_val == NULL) {
    return default_val;
  }
  Int32 result;
  if (!ParseInt32(Message() << "The value of environment variable " << var,
                  str_val, &result)) {
    exit(EXIT_FAILURE);
  }
  return result;
}
bool ShouldRunTestOnShard(int total_shards, int shard_index, int test_id) {
  return (test_id % total_shards) == shard_index;
}
int UnitTestImpl::FilterTests(ReactionToSharding shard_tests) {
  const Int32 total_shards = shard_tests == HONOR_SHARDING_PROTOCOL ?
      Int32FromEnvOrDie(kTestTotalShards, -1) : -1;
  const Int32 shard_index = shard_tests == HONOR_SHARDING_PROTOCOL ?
      Int32FromEnvOrDie(kTestShardIndex, -1) : -1;
  int num_runnable_tests = 0;
  int num_selected_tests = 0;
  for (size_t i = 0; i < test_cases_.size(); i++) {
    TestCase* const test_case = test_cases_[i];
    const std::string &test_case_name = test_case->name();
    test_case->set_should_run(false);
    for (size_t j = 0; j < test_case->test_info_list().size(); j++) {
      TestInfo* const test_info = test_case->test_info_list()[j];
      const std::string test_name(test_info->name());
      const bool is_disabled =
          internal::UnitTestOptions::MatchesFilter(test_case_name,
                                                   kDisableTestFilter) ||
          internal::UnitTestOptions::MatchesFilter(test_name,
                                                   kDisableTestFilter);
      test_info->is_disabled_ = is_disabled;
      const bool matches_filter =
          internal::UnitTestOptions::FilterMatchesTest(test_case_name,
                                                       test_name);
      test_info->matches_filter_ = matches_filter;
      const bool is_runnable =
          (GTEST_FLAG(also_run_disabled_tests) || !is_disabled) &&
          matches_filter;
      const bool is_selected = is_runnable &&
          (shard_tests == IGNORE_SHARDING_PROTOCOL ||
           ShouldRunTestOnShard(total_shards, shard_index,
                                num_runnable_tests));
      num_runnable_tests += is_runnable;
      num_selected_tests += is_selected;
      test_info->should_run_ = is_selected;
      test_case->set_should_run(test_case->should_run() || is_selected);
    }
  }
  return num_selected_tests;
}
static void PrintOnOneLine(const char* str, int max_length) {
  if (str != NULL) {
    for (int i = 0; *str != '\0'; ++str) {
      if (i >= max_length) {
        printf("...");
        break;
      }
      if (*str == '\n') {
        printf("\\n");
        i += 2;
      } else {
        printf("%c", *str);
        ++i;
      }
    }
  }
}
void UnitTestImpl::ListTestsMatchingFilter() {
  const int kMaxParamLength = 250;
  for (size_t i = 0; i < test_cases_.size(); i++) {
    const TestCase* const test_case = test_cases_[i];
    bool printed_test_case_name = false;
    for (size_t j = 0; j < test_case->test_info_list().size(); j++) {
      const TestInfo* const test_info =
          test_case->test_info_list()[j];
      if (test_info->matches_filter_) {
        if (!printed_test_case_name) {
          printed_test_case_name = true;
          printf("%s.", test_case->name());
          if (test_case->type_param() != NULL) {
            printf("  # %s = ", kTypeParamLabel);
            PrintOnOneLine(test_case->type_param(), kMaxParamLength);
          }
          printf("\n");
        }
        printf("  %s", test_info->name());
        if (test_info->value_param() != NULL) {
          printf("  # %s = ", kValueParamLabel);
          PrintOnOneLine(test_info->value_param(), kMaxParamLength);
        }
        printf("\n");
      }
    }
  }
  fflush(stdout);
}
void UnitTestImpl::set_os_stack_trace_getter(
    OsStackTraceGetterInterface* getter) {
  if (os_stack_trace_getter_ != getter) {
    delete os_stack_trace_getter_;
    os_stack_trace_getter_ = getter;
  }
}
OsStackTraceGetterInterface* UnitTestImpl::os_stack_trace_getter() {
  if (os_stack_trace_getter_ == NULL) {
    os_stack_trace_getter_ = new OsStackTraceGetter;
  }
  return os_stack_trace_getter_;
}
TestResult* UnitTestImpl::current_test_result() {
  return current_test_info_ ?
      &(current_test_info_->result_) : &ad_hoc_test_result_;
}
void UnitTestImpl::ShuffleTests() {
  ShuffleRange(random(), 0, last_death_test_case_ + 1, &test_case_indices_);
  ShuffleRange(random(), last_death_test_case_ + 1,
               static_cast<int>(test_cases_.size()), &test_case_indices_);
  for (size_t i = 0; i < test_cases_.size(); i++) {
    test_cases_[i]->ShuffleTests(random());
  }
}
void UnitTestImpl::UnshuffleTests() {
  for (size_t i = 0; i < test_cases_.size(); i++) {
    test_cases_[i]->UnshuffleTests();
    test_case_indices_[i] = static_cast<int>(i);
  }
}
std::string GetCurrentOsStackTraceExceptTop(UnitTest* ,
                                            int skip_count) {
  return GetUnitTestImpl()->CurrentOsStackTraceExceptTop(skip_count + 1);
}
namespace {
class ClassUniqueToAlwaysTrue {};
}
bool IsTrue(bool condition) { return condition; }
bool AlwaysTrue() {
#if GTEST_HAS_EXCEPTIONS
  if (IsTrue(false))
    throw ClassUniqueToAlwaysTrue();
#endif
  return true;
}
bool SkipPrefix(const char* prefix, const char** pstr) {
  const size_t prefix_len = strlen(prefix);
  if (strncmp(*pstr, prefix, prefix_len) == 0) {
    *pstr += prefix_len;
    return true;
  }
  return false;
}
const char* ParseFlagValue(const char* str,
                           const char* flag,
                           bool def_optional) {
  if (str == NULL || flag == NULL) return NULL;
  const std::string flag_str = std::string("--") + GTEST_FLAG_PREFIX_ + flag;
  const size_t flag_len = flag_str.length();
  if (strncmp(str, flag_str.c_str(), flag_len) != 0) return NULL;
  const char* flag_end = str + flag_len;
  if (def_optional && (flag_end[0] == '\0')) {
    return flag_end;
  }
  if (flag_end[0] != '=') return NULL;
  return flag_end + 1;
}
bool ParseBoolFlag(const char* str, const char* flag, bool* value) {
  const char* const value_str = ParseFlagValue(str, flag, true);
  if (value_str == NULL) return false;
  *value = !(*value_str == '0' || *value_str == 'f' || *value_str == 'F');
  return true;
}
bool ParseInt32Flag(const char* str, const char* flag, Int32* value) {
  const char* const value_str = ParseFlagValue(str, flag, false);
  if (value_str == NULL) return false;
  return ParseInt32(Message() << "The value of flag --" << flag,
                    value_str, value);
}
bool ParseStringFlag(const char* str, const char* flag, std::string* value) {
  const char* const value_str = ParseFlagValue(str, flag, false);
  if (value_str == NULL) return false;
  *value = value_str;
  return true;
}
static bool HasGoogleTestFlagPrefix(const char* str) {
  return (SkipPrefix("--", &str) ||
          SkipPrefix("-", &str) ||
          SkipPrefix("/", &str)) &&
         !SkipPrefix(GTEST_FLAG_PREFIX_ "internal_", &str) &&
         (SkipPrefix(GTEST_FLAG_PREFIX_, &str) ||
          SkipPrefix(GTEST_FLAG_PREFIX_DASH_, &str));
}
static void PrintColorEncoded(const char* str) {
  GTestColor color = COLOR_DEFAULT;
  for (;;) {
    const char* p = strchr(str, '@');
    if (p == NULL) {
      ColoredPrintf(color, "%s", str);
      return;
    }
    ColoredPrintf(color, "%s", std::string(str, p).c_str());
    const char ch = p[1];
    str = p + 2;
    if (ch == '@') {
      ColoredPrintf(color, "@");
    } else if (ch == 'D') {
      color = COLOR_DEFAULT;
    } else if (ch == 'R') {
      color = COLOR_RED;
    } else if (ch == 'G') {
      color = COLOR_GREEN;
    } else if (ch == 'Y') {
      color = COLOR_YELLOW;
    } else {
      --str;
    }
  }
}
static const char kColorEncodedHelpMessage[] =
"This program contains tests written using " GTEST_NAME_ ". You can use the\n"
"following command line flags to control its behavior:\n"
"\n"
"Test Selection:\n"
"  @G--" GTEST_FLAG_PREFIX_ "list_tests@D\n"
"      List the names of all tests instead of running them. The name of\n"
"      TEST(Foo, Bar) is \"Foo.Bar\".\n"
"  @G--" GTEST_FLAG_PREFIX_ "filter=@YPOSTIVE_PATTERNS"
    "[@G-@YNEGATIVE_PATTERNS]@D\n"
"      Run only the tests whose name matches one of the positive patterns but\n"
"      none of the negative patterns. '?' matches any single character; '*'\n"
"      matches any substring; ':' separates two patterns.\n"
"  @G--" GTEST_FLAG_PREFIX_ "also_run_disabled_tests@D\n"
"      Run all disabled tests too.\n"
"\n"
"Test Execution:\n"
"  @G--" GTEST_FLAG_PREFIX_ "repeat=@Y[COUNT]@D\n"
"      Run the tests repeatedly; use a negative count to repeat forever.\n"
"  @G--" GTEST_FLAG_PREFIX_ "shuffle@D\n"
"      Randomize tests' orders on every iteration.\n"
"  @G--" GTEST_FLAG_PREFIX_ "random_seed=@Y[NUMBER]@D\n"
"      Random number seed to use for shuffling test orders (between 1 and\n"
"      99999, or 0 to use a seed based on the current time).\n"
"\n"
"Test Output:\n"
"  @G--" GTEST_FLAG_PREFIX_ "color=@Y(@Gyes@Y|@Gno@Y|@Gauto@Y)@D\n"
"      Enable/disable colored output. The default is @Gauto@D.\n"
"  -@G-" GTEST_FLAG_PREFIX_ "print_time=0@D\n"
"      Don't print the elapsed time of each test.\n"
"  @G--" GTEST_FLAG_PREFIX_ "output=xml@Y[@G:@YDIRECTORY_PATH@G"
    GTEST_PATH_SEP_ "@Y|@G:@YFILE_PATH]@D\n"
"      Generate an XML report in the given directory or with the given file\n"
"      name. @YFILE_PATH@D defaults to @Gtest_details.xml@D.\n"
#if GTEST_CAN_STREAM_RESULTS_
"  @G--" GTEST_FLAG_PREFIX_ "stream_result_to=@YHOST@G:@YPORT@D\n"
"      Stream test results to the given server.\n"
#endif
"\n"
"Assertion Behavior:\n"
#if GTEST_HAS_DEATH_TEST && !GTEST_OS_WINDOWS
"  @G--" GTEST_FLAG_PREFIX_ "death_test_style=@Y(@Gfast@Y|@Gthreadsafe@Y)@D\n"
"      Set the default death test style.\n"
#endif
"  @G--" GTEST_FLAG_PREFIX_ "break_on_failure@D\n"
"      Turn assertion failures into debugger break-points.\n"
"  @G--" GTEST_FLAG_PREFIX_ "throw_on_failure@D\n"
"      Turn assertion failures into C++ exceptions.\n"
"  @G--" GTEST_FLAG_PREFIX_ "catch_exceptions=0@D\n"
"      Do not report exceptions as test failures. Instead, allow them\n"
"      to crash the program or throw a pop-up (on Windows).\n"
"\n"
"Except for @G--" GTEST_FLAG_PREFIX_ "list_tests@D, you can alternatively set "
    "the corresponding\n"
"environment variable of a flag (all letters in upper-case). For example, to\n"
"disable colored text output, you can either specify @G--" GTEST_FLAG_PREFIX_
    "color=no@D or set\n"
"the @G" GTEST_FLAG_PREFIX_UPPER_ "COLOR@D environment variable to @Gno@D.\n"
"\n"
"For more information, please read the " GTEST_NAME_ " documentation at\n"
"@G" GTEST_PROJECT_URL_ "@D. If you find a bug in " GTEST_NAME_ "\n"
"(not one in your own code or tests), please report it to\n"
"@G<" GTEST_DEV_EMAIL_ ">@D.\n";
template <typename CharType>
void ParseGoogleTestFlagsOnlyImpl(int* argc, CharType** argv) {
  for (int i = 1; i < *argc; i++) {
    const std::string arg_string = StreamableToString(argv[i]);
    const char* const arg = arg_string.c_str();
    using internal::ParseBoolFlag;
    using internal::ParseInt32Flag;
    using internal::ParseStringFlag;
    if (ParseBoolFlag(arg, kAlsoRunDisabledTestsFlag,
                      &GTEST_FLAG(also_run_disabled_tests)) ||
        ParseBoolFlag(arg, kBreakOnFailureFlag,
                      &GTEST_FLAG(break_on_failure)) ||
        ParseBoolFlag(arg, kCatchExceptionsFlag,
                      &GTEST_FLAG(catch_exceptions)) ||
        ParseStringFlag(arg, kColorFlag, &GTEST_FLAG(color)) ||
        ParseStringFlag(arg, kDeathTestStyleFlag,
                        &GTEST_FLAG(death_test_style)) ||
        ParseBoolFlag(arg, kDeathTestUseFork,
                      &GTEST_FLAG(death_test_use_fork)) ||
        ParseStringFlag(arg, kFilterFlag, &GTEST_FLAG(filter)) ||
        ParseStringFlag(arg, kInternalRunDeathTestFlag,
                        &GTEST_FLAG(internal_run_death_test)) ||
        ParseBoolFlag(arg, kListTestsFlag, &GTEST_FLAG(list_tests)) ||
        ParseStringFlag(arg, kOutputFlag, &GTEST_FLAG(output)) ||
        ParseBoolFlag(arg, kPrintTimeFlag, &GTEST_FLAG(print_time)) ||
        ParseInt32Flag(arg, kRandomSeedFlag, &GTEST_FLAG(random_seed)) ||
        ParseInt32Flag(arg, kRepeatFlag, &GTEST_FLAG(repeat)) ||
        ParseBoolFlag(arg, kShuffleFlag, &GTEST_FLAG(shuffle)) ||
        ParseInt32Flag(arg, kStackTraceDepthFlag,
                       &GTEST_FLAG(stack_trace_depth)) ||
        ParseStringFlag(arg, kStreamResultToFlag,
                        &GTEST_FLAG(stream_result_to)) ||
        ParseBoolFlag(arg, kThrowOnFailureFlag,
                      &GTEST_FLAG(throw_on_failure))
        ) {
      for (int j = i; j != *argc; j++) {
        argv[j] = argv[j + 1];
      }
      (*argc)--;
      i--;
    } else if (arg_string == "--help" || arg_string == "-h" ||
               arg_string == "-?" || arg_string == "/?" ||
               HasGoogleTestFlagPrefix(arg)) {
      g_help_flag = true;
    }
  }
  if (g_help_flag) {
    PrintColorEncoded(kColorEncodedHelpMessage);
  }
}
void ParseGoogleTestFlagsOnly(int* argc, char** argv) {
  ParseGoogleTestFlagsOnlyImpl(argc, argv);
}
void ParseGoogleTestFlagsOnly(int* argc, wchar_t** argv) {
  ParseGoogleTestFlagsOnlyImpl(argc, argv);
}
template <typename CharType>
void InitGoogleTestImpl(int* argc, CharType** argv) {
  g_init_gtest_count++;
  if (g_init_gtest_count != 1) return;
  if (*argc <= 0) return;
  internal::g_executable_path = internal::StreamableToString(argv[0]);
#if GTEST_HAS_DEATH_TEST
  g_argvs.clear();
  for (int i = 0; i != *argc; i++) {
    g_argvs.push_back(StreamableToString(argv[i]));
  }
#endif
  ParseGoogleTestFlagsOnly(argc, argv);
  GetUnitTestImpl()->PostFlagParsingInit();
}
}
void InitGoogleTest(int* argc, char** argv) {
  internal::InitGoogleTestImpl(argc, argv);
}
void InitGoogleTest(int* argc, wchar_t** argv) {
  internal::InitGoogleTestImpl(argc, argv);
}
}
#if GTEST_HAS_DEATH_TEST
# if GTEST_OS_MAC
# include <crt_externs.h>
# endif
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# if GTEST_OS_LINUX
# include <signal.h>
# endif
# include <stdarg.h>
# if GTEST_OS_WINDOWS
# include <windows.h>
# else
# include <sys/mman.h>
# include <sys/wait.h>
# endif
# if GTEST_OS_QNX
# include <spawn.h>
# endif
#endif
#define GTEST_IMPLEMENTATION_ 1
#undef GTEST_IMPLEMENTATION_
namespace testing {
static const char kDefaultDeathTestStyle[] = "fast";
GTEST_DEFINE_string_(
    death_test_style,
    internal::StringFromGTestEnv("death_test_style", kDefaultDeathTestStyle),
    "Indicates how to run a death test in a forked child process: "
    "\"threadsafe\" (child process re-executes the test binary "
    "from the beginning, running only the specific death test) or "
    "\"fast\" (child process runs the death test immediately "
    "after forking).");
GTEST_DEFINE_bool_(
    death_test_use_fork,
    internal::BoolFromGTestEnv("death_test_use_fork", false),
    "Instructs to use fork()/_exit() instead of clone() in death tests. "
    "Ignored and always uses fork() on POSIX systems where clone() is not "
    "implemented. Useful when running under valgrind or similar tools if "
    "those do not support clone(). Valgrind 3.3.1 will just fail if "
    "it sees an unsupported combination of clone() flags. "
    "It is not recommended to use this flag w/o valgrind though it will "
    "work in 99% of the cases. Once valgrind is fixed, this flag will "
    "most likely be removed.");
namespace internal {
GTEST_DEFINE_string_(
    internal_run_death_test, "",
    "Indicates the file, line number, temporal index of "
    "the single death test to run, and a file descriptor to "
    "which a success code may be sent, all separated by "
    "the '|' characters.  This flag is specified if and only if the current "
    "process is a sub-process launched for running a thread-safe "
    "death test.  FOR INTERNAL USE ONLY.");
}
#if GTEST_HAS_DEATH_TEST
namespace internal {
static bool g_in_fast_death_test_child = false;
bool InDeathTestChild() {
# if GTEST_OS_WINDOWS
  return !GTEST_FLAG(internal_run_death_test).empty();
# else
  if (GTEST_FLAG(death_test_style) == "threadsafe")
    return !GTEST_FLAG(internal_run_death_test).empty();
  else
    return g_in_fast_death_test_child;
#endif
}
}
ExitedWithCode::ExitedWithCode(int exit_code) : exit_code_(exit_code) {
}
bool ExitedWithCode::operator()(int exit_status) const {
# if GTEST_OS_WINDOWS
  return exit_status == exit_code_;
# else
  return WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == exit_code_;
# endif
}
# if !GTEST_OS_WINDOWS
KilledBySignal::KilledBySignal(int signum) : signum_(signum) {
}
bool KilledBySignal::operator()(int exit_status) const {
  return WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == signum_;
}
# endif
namespace internal {
static std::string ExitSummary(int exit_code) {
  Message m;
# if GTEST_OS_WINDOWS
  m << "Exited with exit status " << exit_code;
# else
  if (WIFEXITED(exit_code)) {
    m << "Exited with exit status " << WEXITSTATUS(exit_code);
  } else if (WIFSIGNALED(exit_code)) {
    m << "Terminated by signal " << WTERMSIG(exit_code);
  }
# ifdef WCOREDUMP
  if (WCOREDUMP(exit_code)) {
    m << " (core dumped)";
  }
# endif
# endif
  return m.GetString();
}
bool ExitedUnsuccessfully(int exit_status) {
  return !ExitedWithCode(0)(exit_status);
}
# if !GTEST_OS_WINDOWS
static std::string DeathTestThreadWarning(size_t thread_count) {
  Message msg;
  msg << "Death tests use fork(), which is unsafe particularly"
      << " in a threaded context. For this test, " << GTEST_NAME_ << " ";
  if (thread_count == 0)
    msg << "couldn't detect the number of threads.";
  else
    msg << "detected " << thread_count << " threads.";
  return msg.GetString();
}
# endif
static const char kDeathTestLived = 'L';
static const char kDeathTestReturned = 'R';
static const char kDeathTestThrew = 'T';
static const char kDeathTestInternalError = 'I';
enum DeathTestOutcome { IN_PROGRESS, DIED, LIVED, RETURNED, THREW };
void DeathTestAbort(const std::string& message) {
  const InternalRunDeathTestFlag* const flag =
      GetUnitTestImpl()->internal_run_death_test_flag();
  if (flag != NULL) {
    FILE* parent = posix::FDOpen(flag->write_fd(), "w");
    fputc(kDeathTestInternalError, parent);
    fprintf(parent, "%s", message.c_str());
    fflush(parent);
    _exit(1);
  } else {
    fprintf(stderr, "%s", message.c_str());
    fflush(stderr);
    posix::Abort();
  }
}
#define GTEST_DEATH_TEST_CHECK_(expression) \
  do { \
    if (!::testing::internal::IsTrue(expression)) { \
      DeathTestAbort( \
          ::std::string("CHECK failed: File ") + __FILE__ + ", line " \
          + ::testing::internal::StreamableToString(__LINE__) + ": " \
          + #expression); \
    } \
  } while (::testing::internal::AlwaysFalse())
#define GTEST_DEATH_TEST_CHECK_SYSCALL_(expression) \
  do { \
    int gtest_retval; \
    do { \
      gtest_retval = (expression); \
    } while (gtest_retval == -1 && errno == EINTR); \
    if (gtest_retval == -1) { \
      DeathTestAbort( \
          ::std::string("CHECK failed: File ") + __FILE__ + ", line " \
          + ::testing::internal::StreamableToString(__LINE__) + ": " \
          + #expression + " != -1"); \
    } \
  } while (::testing::internal::AlwaysFalse())
std::string GetLastErrnoDescription() {
    return errno == 0 ? "" : posix::StrError(errno);
}
static void FailFromInternalError(int fd) {
  Message error;
  char buffer[256];
  int num_read;
  do {
    while ((num_read = posix::Read(fd, buffer, 255)) > 0) {
      buffer[num_read] = '\0';
      error << buffer;
    }
  } while (num_read == -1 && errno == EINTR);
  if (num_read == 0) {
    GTEST_LOG_(FATAL) << error.GetString();
  } else {
    const int last_error = errno;
    GTEST_LOG_(FATAL) << "Error while reading death test internal: "
                      << GetLastErrnoDescription() << " [" << last_error << "]";
  }
}
DeathTest::DeathTest() {
  TestInfo* const info = GetUnitTestImpl()->current_test_info();
  if (info == NULL) {
    DeathTestAbort("Cannot run a death test outside of a TEST or "
                   "TEST_F construct");
  }
}
bool DeathTest::Create(const char* statement, const RE* regex,
                       const char* file, int line, DeathTest** test) {
  return GetUnitTestImpl()->death_test_factory()->Create(
      statement, regex, file, line, test);
}
const char* DeathTest::LastMessage() {
  return last_death_test_message_.c_str();
}
void DeathTest::set_last_death_test_message(const std::string& message) {
  last_death_test_message_ = message;
}
std::string DeathTest::last_death_test_message_;
class DeathTestImpl : public DeathTest {
 protected:
  DeathTestImpl(const char* a_statement, const RE* a_regex)
      : statement_(a_statement),
        regex_(a_regex),
        spawned_(false),
        status_(-1),
        outcome_(IN_PROGRESS),
        read_fd_(-1),
        write_fd_(-1) {}
  ~DeathTestImpl() { GTEST_DEATH_TEST_CHECK_(read_fd_ == -1); }
  void Abort(AbortReason reason);
  virtual bool Passed(bool status_ok);
  const char* statement() const { return statement_; }
  const RE* regex() const { return regex_; }
  bool spawned() const { return spawned_; }
  void set_spawned(bool is_spawned) { spawned_ = is_spawned; }
  int status() const { return status_; }
  void set_status(int a_status) { status_ = a_status; }
  DeathTestOutcome outcome() const { return outcome_; }
  void set_outcome(DeathTestOutcome an_outcome) { outcome_ = an_outcome; }
  int read_fd() const { return read_fd_; }
  void set_read_fd(int fd) { read_fd_ = fd; }
  int write_fd() const { return write_fd_; }
  void set_write_fd(int fd) { write_fd_ = fd; }
  void ReadAndInterpretStatusByte();
 private:
  const char* const statement_;
  const RE* const regex_;
  bool spawned_;
  int status_;
  DeathTestOutcome outcome_;
  int read_fd_;
  int write_fd_;
};
void DeathTestImpl::ReadAndInterpretStatusByte() {
  char flag;
  int bytes_read;
  do {
    bytes_read = posix::Read(read_fd(), &flag, 1);
  } while (bytes_read == -1 && errno == EINTR);
  if (bytes_read == 0) {
    set_outcome(DIED);
  } else if (bytes_read == 1) {
    switch (flag) {
      case kDeathTestReturned:
        set_outcome(RETURNED);
        break;
      case kDeathTestThrew:
        set_outcome(THREW);
        break;
      case kDeathTestLived:
        set_outcome(LIVED);
        break;
      case kDeathTestInternalError:
        FailFromInternalError(read_fd());
        break;
      default:
        GTEST_LOG_(FATAL) << "Death test child process reported "
                          << "unexpected status byte ("
                          << static_cast<unsigned int>(flag) << ")";
    }
  } else {
    GTEST_LOG_(FATAL) << "Read from death test child process failed: "
                      << GetLastErrnoDescription();
  }
  GTEST_DEATH_TEST_CHECK_SYSCALL_(posix::Close(read_fd()));
  set_read_fd(-1);
}
void DeathTestImpl::Abort(AbortReason reason) {
  const char status_ch =
      reason == TEST_DID_NOT_DIE ? kDeathTestLived :
      reason == TEST_THREW_EXCEPTION ? kDeathTestThrew : kDeathTestReturned;
  GTEST_DEATH_TEST_CHECK_SYSCALL_(posix::Write(write_fd(), &status_ch, 1));
  _exit(1);
}
static ::std::string FormatDeathTestOutput(const ::std::string& output) {
  ::std::string ret;
  for (size_t at = 0; ; ) {
    const size_t line_end = output.find('\n', at);
    ret += "[  DEATH   ] ";
    if (line_end == ::std::string::npos) {
      ret += output.substr(at);
      break;
    }
    ret += output.substr(at, line_end + 1 - at);
    at = line_end + 1;
  }
  return ret;
}
bool DeathTestImpl::Passed(bool status_ok) {
  if (!spawned())
    return false;
  const std::string error_message = GetCapturedStderr();
  bool success = false;
  Message buffer;
  buffer << "Death test: " << statement() << "\n";
  switch (outcome()) {
    case LIVED:
      buffer << "    Result: failed to die.\n"
             << " Error msg:\n" << FormatDeathTestOutput(error_message);
      break;
    case THREW:
      buffer << "    Result: threw an exception.\n"
             << " Error msg:\n" << FormatDeathTestOutput(error_message);
      break;
    case RETURNED:
      buffer << "    Result: illegal return in test statement.\n"
             << " Error msg:\n" << FormatDeathTestOutput(error_message);
      break;
    case DIED:
      if (status_ok) {
        const bool matched = RE::PartialMatch(error_message.c_str(), *regex());
        if (matched) {
          success = true;
        } else {
          buffer << "    Result: died but not with expected error.\n"
                 << "  Expected: " << regex()->pattern() << "\n"
                 << "Actual msg:\n" << FormatDeathTestOutput(error_message);
        }
      } else {
        buffer << "    Result: died but not with expected exit code:\n"
               << "            " << ExitSummary(status()) << "\n"
               << "Actual msg:\n" << FormatDeathTestOutput(error_message);
      }
      break;
    case IN_PROGRESS:
    default:
      GTEST_LOG_(FATAL)
          << "DeathTest::Passed somehow called before conclusion of test";
  }
  DeathTest::set_last_death_test_message(buffer.GetString());
  return success;
}
# if GTEST_OS_WINDOWS
class WindowsDeathTest : public DeathTestImpl {
 public:
  WindowsDeathTest(const char* a_statement,
                   const RE* a_regex,
                   const char* file,
                   int line)
      : DeathTestImpl(a_statement, a_regex), file_(file), line_(line) {}
  virtual int Wait();
  virtual TestRole AssumeRole();
 private:
  const char* const file_;
  const int line_;
  AutoHandle write_handle_;
  AutoHandle child_handle_;
  AutoHandle event_handle_;
};
int WindowsDeathTest::Wait() {
  if (!spawned())
    return 0;
  const HANDLE wait_handles[2] = { child_handle_.Get(), event_handle_.Get() };
  switch (::WaitForMultipleObjects(2,
                                   wait_handles,
                                   FALSE,
                                   INFINITE)) {
    case WAIT_OBJECT_0:
    case WAIT_OBJECT_0 + 1:
      break;
    default:
      GTEST_DEATH_TEST_CHECK_(false);
  }
  write_handle_.Reset();
  event_handle_.Reset();
  ReadAndInterpretStatusByte();
  GTEST_DEATH_TEST_CHECK_(
      WAIT_OBJECT_0 == ::WaitForSingleObject(child_handle_.Get(),
                                             INFINITE));
  DWORD status_code;
  GTEST_DEATH_TEST_CHECK_(
      ::GetExitCodeProcess(child_handle_.Get(), &status_code) != FALSE);
  child_handle_.Reset();
  set_status(static_cast<int>(status_code));
  return status();
}
DeathTest::TestRole WindowsDeathTest::AssumeRole() {
  const UnitTestImpl* const impl = GetUnitTestImpl();
  const InternalRunDeathTestFlag* const flag =
      impl->internal_run_death_test_flag();
  const TestInfo* const info = impl->current_test_info();
  const int death_test_index = info->result()->death_test_count();
  if (flag != NULL) {
    set_write_fd(flag->write_fd());
    return EXECUTE_TEST;
  }
  SECURITY_ATTRIBUTES handles_are_inheritable = {
    sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
  HANDLE read_handle, write_handle;
  GTEST_DEATH_TEST_CHECK_(
      ::CreatePipe(&read_handle, &write_handle, &handles_are_inheritable,
                   0)
      != FALSE);
  set_read_fd(::_open_osfhandle(reinterpret_cast<intptr_t>(read_handle),
                                O_RDONLY));
  write_handle_.Reset(write_handle);
  event_handle_.Reset(::CreateEvent(
      &handles_are_inheritable,
      TRUE,
      FALSE,
      NULL));
  GTEST_DEATH_TEST_CHECK_(event_handle_.Get() != NULL);
  const std::string filter_flag =
      std::string("--") + GTEST_FLAG_PREFIX_ + kFilterFlag + "=" +
      info->test_case_name() + "." + info->name();
  const std::string internal_flag =
      std::string("--") + GTEST_FLAG_PREFIX_ + kInternalRunDeathTestFlag +
      "=" + file_ + "|" + StreamableToString(line_) + "|" +
      StreamableToString(death_test_index) + "|" +
      StreamableToString(static_cast<unsigned int>(::GetCurrentProcessId())) +
      "|" + StreamableToString(reinterpret_cast<size_t>(write_handle)) +
      "|" + StreamableToString(reinterpret_cast<size_t>(event_handle_.Get()));
  char executable_path[_MAX_PATH + 1];
  GTEST_DEATH_TEST_CHECK_(
      _MAX_PATH + 1 != ::GetModuleFileNameA(NULL,
                                            executable_path,
                                            _MAX_PATH));
  std::string command_line =
      std::string(::GetCommandLineA()) + " " + filter_flag + " \"" +
      internal_flag + "\"";
  DeathTest::set_last_death_test_message("");
  CaptureStderr();
  FlushInfoLog();
  STARTUPINFOA startup_info;
  memset(&startup_info, 0, sizeof(STARTUPINFO));
  startup_info.dwFlags = STARTF_USESTDHANDLES;
  startup_info.hStdInput = ::GetStdHandle(STD_INPUT_HANDLE);
  startup_info.hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
  startup_info.hStdError = ::GetStdHandle(STD_ERROR_HANDLE);
  PROCESS_INFORMATION process_info;
  GTEST_DEATH_TEST_CHECK_(::CreateProcessA(
      executable_path,
      const_cast<char*>(command_line.c_str()),
      NULL,
      NULL,
      TRUE,
      0x0,
      NULL,
      UnitTest::GetInstance()->original_working_dir(),
      &startup_info,
      &process_info) != FALSE);
  child_handle_.Reset(process_info.hProcess);
  ::CloseHandle(process_info.hThread);
  set_spawned(true);
  return OVERSEE_TEST;
}
# else
class ForkingDeathTest : public DeathTestImpl {
 public:
  ForkingDeathTest(const char* statement, const RE* regex);
  virtual int Wait();
 protected:
  void set_child_pid(pid_t child_pid) { child_pid_ = child_pid; }
 private:
  pid_t child_pid_;
};
ForkingDeathTest::ForkingDeathTest(const char* a_statement, const RE* a_regex)
    : DeathTestImpl(a_statement, a_regex),
      child_pid_(-1) {}
int ForkingDeathTest::Wait() {
  if (!spawned())
    return 0;
  ReadAndInterpretStatusByte();
  int status_value;
  GTEST_DEATH_TEST_CHECK_SYSCALL_(waitpid(child_pid_, &status_value, 0));
  set_status(status_value);
  return status_value;
}
class NoExecDeathTest : public ForkingDeathTest {
 public:
  NoExecDeathTest(const char* a_statement, const RE* a_regex) :
      ForkingDeathTest(a_statement, a_regex) { }
  virtual TestRole AssumeRole();
};
DeathTest::TestRole NoExecDeathTest::AssumeRole() {
  const size_t thread_count = GetThreadCount();
  if (thread_count != 1) {
    GTEST_LOG_(WARNING) << DeathTestThreadWarning(thread_count);
  }
  int pipe_fd[2];
  GTEST_DEATH_TEST_CHECK_(pipe(pipe_fd) != -1);
  DeathTest::set_last_death_test_message("");
  CaptureStderr();
  FlushInfoLog();
  const pid_t child_pid = fork();
  GTEST_DEATH_TEST_CHECK_(child_pid != -1);
  set_child_pid(child_pid);
  if (child_pid == 0) {
    GTEST_DEATH_TEST_CHECK_SYSCALL_(close(pipe_fd[0]));
    set_write_fd(pipe_fd[1]);
    LogToStderr();
    GetUnitTestImpl()->listeners()->SuppressEventForwarding();
    g_in_fast_death_test_child = true;
    return EXECUTE_TEST;
  } else {
    GTEST_DEATH_TEST_CHECK_SYSCALL_(close(pipe_fd[1]));
    set_read_fd(pipe_fd[0]);
    set_spawned(true);
    return OVERSEE_TEST;
  }
}
class ExecDeathTest : public ForkingDeathTest {
 public:
  ExecDeathTest(const char* a_statement, const RE* a_regex,
                const char* file, int line) :
      ForkingDeathTest(a_statement, a_regex), file_(file), line_(line) { }
  virtual TestRole AssumeRole();
 private:
  static ::std::vector<testing::internal::string>
  GetArgvsForDeathTestChildProcess() {
    ::std::vector<testing::internal::string> args = GetInjectableArgvs();
    return args;
  }
  const char* const file_;
  const int line_;
};
class Arguments {
 public:
  Arguments() {
    args_.push_back(NULL);
  }
  ~Arguments() {
    for (std::vector<char*>::iterator i = args_.begin(); i != args_.end();
         ++i) {
      free(*i);
    }
  }
  void AddArgument(const char* argument) {
    args_.insert(args_.end() - 1, posix::StrDup(argument));
  }
  template <typename Str>
  void AddArguments(const ::std::vector<Str>& arguments) {
    for (typename ::std::vector<Str>::const_iterator i = arguments.begin();
         i != arguments.end();
         ++i) {
      args_.insert(args_.end() - 1, posix::StrDup(i->c_str()));
    }
  }
  char* const* Argv() {
    return &args_[0];
  }
 private:
  std::vector<char*> args_;
};
struct ExecDeathTestArgs {
  char* const* argv;
  int close_fd;
};
# if GTEST_OS_MAC
inline char** GetEnviron() {
  return *_NSGetEnviron();
}
# else
extern "C" char** environ;
inline char** GetEnviron() { return environ; }
# endif
# if !GTEST_OS_QNX
static int ExecDeathTestChildMain(void* child_arg) {
  ExecDeathTestArgs* const args = static_cast<ExecDeathTestArgs*>(child_arg);
  GTEST_DEATH_TEST_CHECK_SYSCALL_(close(args->close_fd));
  const char* const original_dir =
      UnitTest::GetInstance()->original_working_dir();
  if (chdir(original_dir) != 0) {
    DeathTestAbort(std::string("chdir(\"") + original_dir + "\") failed: " +
                   GetLastErrnoDescription());
    return EXIT_FAILURE;
  }
  execve(args->argv[0], args->argv, GetEnviron());
  DeathTestAbort(std::string("execve(") + args->argv[0] + ", ...) in " +
                 original_dir + " failed: " +
                 GetLastErrnoDescription());
  return EXIT_FAILURE;
}
# endif
void StackLowerThanAddress(const void* ptr, bool* result) GTEST_NO_INLINE_;
void StackLowerThanAddress(const void* ptr, bool* result) {
  int dummy;
  *result = (&dummy < ptr);
}
bool StackGrowsDown() {
  int dummy;
  bool result;
  StackLowerThanAddress(&dummy, &result);
  return result;
}
static pid_t ExecDeathTestSpawnChild(char* const* argv, int close_fd) {
  ExecDeathTestArgs args = { argv, close_fd };
  pid_t child_pid = -1;
# if GTEST_OS_QNX
  const int cwd_fd = open(".", O_RDONLY);
  GTEST_DEATH_TEST_CHECK_(cwd_fd != -1);
  GTEST_DEATH_TEST_CHECK_SYSCALL_(fcntl(cwd_fd, F_SETFD, FD_CLOEXEC));
  const char* const original_dir =
      UnitTest::GetInstance()->original_working_dir();
  if (chdir(original_dir) != 0) {
    DeathTestAbort(std::string("chdir(\"") + original_dir + "\") failed: " +
                   GetLastErrnoDescription());
    return EXIT_FAILURE;
  }
  int fd_flags;
  GTEST_DEATH_TEST_CHECK_SYSCALL_(fd_flags = fcntl(close_fd, F_GETFD));
  GTEST_DEATH_TEST_CHECK_SYSCALL_(fcntl(close_fd, F_SETFD,
                                        fd_flags | FD_CLOEXEC));
  struct inheritance inherit = {0};
  child_pid = spawn(args.argv[0], 0, NULL, &inherit, args.argv, GetEnviron());
  GTEST_DEATH_TEST_CHECK_(fchdir(cwd_fd) != -1);
  GTEST_DEATH_TEST_CHECK_SYSCALL_(close(cwd_fd));
# else
# if GTEST_OS_LINUX
  struct sigaction saved_sigprof_action;
  struct sigaction ignore_sigprof_action;
  memset(&ignore_sigprof_action, 0, sizeof(ignore_sigprof_action));
  sigemptyset(&ignore_sigprof_action.sa_mask);
  ignore_sigprof_action.sa_handler = SIG_IGN;
  GTEST_DEATH_TEST_CHECK_SYSCALL_(sigaction(
      SIGPROF, &ignore_sigprof_action, &saved_sigprof_action));
# endif
# if GTEST_HAS_CLONE
  const bool use_fork = GTEST_FLAG(death_test_use_fork);
  if (!use_fork) {
    static const bool stack_grows_down = StackGrowsDown();
    const size_t stack_size = getpagesize();
    void* const stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE,
                             MAP_ANON | MAP_PRIVATE, -1, 0);
    GTEST_DEATH_TEST_CHECK_(stack != MAP_FAILED);
    const size_t kMaxStackAlignment = 64;
    void* const stack_top =
        static_cast<char*>(stack) +
            (stack_grows_down ? stack_size - kMaxStackAlignment : 0);
    GTEST_DEATH_TEST_CHECK_(stack_size > kMaxStackAlignment &&
        reinterpret_cast<intptr_t>(stack_top) % kMaxStackAlignment == 0);
    child_pid = clone(&ExecDeathTestChildMain, stack_top, SIGCHLD, &args);
    GTEST_DEATH_TEST_CHECK_(munmap(stack, stack_size) != -1);
  }
# else
  const bool use_fork = true;
# endif
  if (use_fork && (child_pid = fork()) == 0) {
      ExecDeathTestChildMain(&args);
      _exit(0);
  }
# endif
# if GTEST_OS_LINUX
  GTEST_DEATH_TEST_CHECK_SYSCALL_(
      sigaction(SIGPROF, &saved_sigprof_action, NULL));
# endif
  GTEST_DEATH_TEST_CHECK_(child_pid != -1);
  return child_pid;
}
DeathTest::TestRole ExecDeathTest::AssumeRole() {
  const UnitTestImpl* const impl = GetUnitTestImpl();
  const InternalRunDeathTestFlag* const flag =
      impl->internal_run_death_test_flag();
  const TestInfo* const info = impl->current_test_info();
  const int death_test_index = info->result()->death_test_count();
  if (flag != NULL) {
    set_write_fd(flag->write_fd());
    return EXECUTE_TEST;
  }
  int pipe_fd[2];
  GTEST_DEATH_TEST_CHECK_(pipe(pipe_fd) != -1);
  GTEST_DEATH_TEST_CHECK_(fcntl(pipe_fd[1], F_SETFD, 0) != -1);
  const std::string filter_flag =
      std::string("--") + GTEST_FLAG_PREFIX_ + kFilterFlag + "="
      + info->test_case_name() + "." + info->name();
  const std::string internal_flag =
      std::string("--") + GTEST_FLAG_PREFIX_ + kInternalRunDeathTestFlag + "="
      + file_ + "|" + StreamableToString(line_) + "|"
      + StreamableToString(death_test_index) + "|"
      + StreamableToString(pipe_fd[1]);
  Arguments args;
  args.AddArguments(GetArgvsForDeathTestChildProcess());
  args.AddArgument(filter_flag.c_str());
  args.AddArgument(internal_flag.c_str());
  DeathTest::set_last_death_test_message("");
  CaptureStderr();
  FlushInfoLog();
  const pid_t child_pid = ExecDeathTestSpawnChild(args.Argv(), pipe_fd[0]);
  GTEST_DEATH_TEST_CHECK_SYSCALL_(close(pipe_fd[1]));
  set_child_pid(child_pid);
  set_read_fd(pipe_fd[0]);
  set_spawned(true);
  return OVERSEE_TEST;
}
# endif
bool DefaultDeathTestFactory::Create(const char* statement, const RE* regex,
                                     const char* file, int line,
                                     DeathTest** test) {
  UnitTestImpl* const impl = GetUnitTestImpl();
  const InternalRunDeathTestFlag* const flag =
      impl->internal_run_death_test_flag();
  const int death_test_index = impl->current_test_info()
      ->increment_death_test_count();
  if (flag != NULL) {
    if (death_test_index > flag->index()) {
      DeathTest::set_last_death_test_message(
          "Death test count (" + StreamableToString(death_test_index)
          + ") somehow exceeded expected maximum ("
          + StreamableToString(flag->index()) + ")");
      return false;
    }
    if (!(flag->file() == file && flag->line() == line &&
          flag->index() == death_test_index)) {
      *test = NULL;
      return true;
    }
  }
# if GTEST_OS_WINDOWS
  if (GTEST_FLAG(death_test_style) == "threadsafe" ||
      GTEST_FLAG(death_test_style) == "fast") {
    *test = new WindowsDeathTest(statement, regex, file, line);
  }
# else
  if (GTEST_FLAG(death_test_style) == "threadsafe") {
    *test = new ExecDeathTest(statement, regex, file, line);
  } else if (GTEST_FLAG(death_test_style) == "fast") {
    *test = new NoExecDeathTest(statement, regex);
  }
# endif
  else {
    DeathTest::set_last_death_test_message(
        "Unknown death test style \"" + GTEST_FLAG(death_test_style)
        + "\" encountered");
    return false;
  }
  return true;
}
static void SplitString(const ::std::string& str, char delimiter,
                        ::std::vector< ::std::string>* dest) {
  ::std::vector< ::std::string> parsed;
  ::std::string::size_type pos = 0;
  while (::testing::internal::AlwaysTrue()) {
    const ::std::string::size_type colon = str.find(delimiter, pos);
    if (colon == ::std::string::npos) {
      parsed.push_back(str.substr(pos));
      break;
    } else {
      parsed.push_back(str.substr(pos, colon - pos));
      pos = colon + 1;
    }
  }
  dest->swap(parsed);
}
# if GTEST_OS_WINDOWS
int GetStatusFileDescriptor(unsigned int parent_process_id,
                            size_t write_handle_as_size_t,
                            size_t event_handle_as_size_t) {
  AutoHandle parent_process_handle(::OpenProcess(PROCESS_DUP_HANDLE,
                                                   FALSE,
                                                   parent_process_id));
  if (parent_process_handle.Get() == INVALID_HANDLE_VALUE) {
    DeathTestAbort("Unable to open parent process " +
                   StreamableToString(parent_process_id));
  }
  GTEST_CHECK_(sizeof(HANDLE) <= sizeof(size_t));
  const HANDLE write_handle =
      reinterpret_cast<HANDLE>(write_handle_as_size_t);
  HANDLE dup_write_handle;
  if (!::DuplicateHandle(parent_process_handle.Get(), write_handle,
                         ::GetCurrentProcess(), &dup_write_handle,
                         0x0,
                         FALSE,
                         DUPLICATE_SAME_ACCESS)) {
    DeathTestAbort("Unable to duplicate the pipe handle " +
                   StreamableToString(write_handle_as_size_t) +
                   " from the parent process " +
                   StreamableToString(parent_process_id));
  }
  const HANDLE event_handle = reinterpret_cast<HANDLE>(event_handle_as_size_t);
  HANDLE dup_event_handle;
  if (!::DuplicateHandle(parent_process_handle.Get(), event_handle,
                         ::GetCurrentProcess(), &dup_event_handle,
                         0x0,
                         FALSE,
                         DUPLICATE_SAME_ACCESS)) {
    DeathTestAbort("Unable to duplicate the event handle " +
                   StreamableToString(event_handle_as_size_t) +
                   " from the parent process " +
                   StreamableToString(parent_process_id));
  }
  const int write_fd =
      ::_open_osfhandle(reinterpret_cast<intptr_t>(dup_write_handle), O_APPEND);
  if (write_fd == -1) {
    DeathTestAbort("Unable to convert pipe handle " +
                   StreamableToString(write_handle_as_size_t) +
                   " to a file descriptor");
  }
  ::SetEvent(dup_event_handle);
  return write_fd;
}
# endif
InternalRunDeathTestFlag* ParseInternalRunDeathTestFlag() {
  if (GTEST_FLAG(internal_run_death_test) == "") return NULL;
  int line = -1;
  int index = -1;
  ::std::vector< ::std::string> fields;
  SplitString(GTEST_FLAG(internal_run_death_test).c_str(), '|', &fields);
  int write_fd = -1;
# if GTEST_OS_WINDOWS
  unsigned int parent_process_id = 0;
  size_t write_handle_as_size_t = 0;
  size_t event_handle_as_size_t = 0;
  if (fields.size() != 6
      || !ParseNaturalNumber(fields[1], &line)
      || !ParseNaturalNumber(fields[2], &index)
      || !ParseNaturalNumber(fields[3], &parent_process_id)
      || !ParseNaturalNumber(fields[4], &write_handle_as_size_t)
      || !ParseNaturalNumber(fields[5], &event_handle_as_size_t)) {
    DeathTestAbort("Bad --gtest_internal_run_death_test flag: " +
                   GTEST_FLAG(internal_run_death_test));
  }
  write_fd = GetStatusFileDescriptor(parent_process_id,
                                     write_handle_as_size_t,
                                     event_handle_as_size_t);
# else
  if (fields.size() != 4
      || !ParseNaturalNumber(fields[1], &line)
      || !ParseNaturalNumber(fields[2], &index)
      || !ParseNaturalNumber(fields[3], &write_fd)) {
    DeathTestAbort("Bad --gtest_internal_run_death_test flag: "
        + GTEST_FLAG(internal_run_death_test));
  }
# endif
  return new InternalRunDeathTestFlag(fields[0], line, index, write_fd);
}
}
#endif
}
#include <stdlib.h>
#if GTEST_OS_WINDOWS_MOBILE
# include <windows.h>
#elif GTEST_OS_WINDOWS
# include <direct.h>
# include <io.h>
#elif GTEST_OS_SYMBIAN
# include <sys/syslimits.h>
#else
# include <limits.h>
# include <climits>
#endif
#if GTEST_OS_WINDOWS
#define GTEST_PATH_MAX_ _MAX_PATH
#elif defined(PATH_MAX)
#define GTEST_PATH_MAX_ PATH_MAX
#elif defined(_XOPEN_PATH_MAX)
#define GTEST_PATH_MAX_ _XOPEN_PATH_MAX
#else
#define GTEST_PATH_MAX_ _POSIX_PATH_MAX
#endif
namespace testing {
namespace internal {
#if GTEST_OS_WINDOWS
const char kPathSeparator = '\\';
const char kAlternatePathSeparator = '/';
const char kPathSeparatorString[] = "\\";
const char kAlternatePathSeparatorString[] = "/";
# if GTEST_OS_WINDOWS_MOBILE
const char kCurrentDirectoryString[] = "\\";
const DWORD kInvalidFileAttributes = 0xffffffff;
# else
const char kCurrentDirectoryString[] = ".\\";
# endif
#else
const char kPathSeparator = '/';
const char kPathSeparatorString[] = "/";
const char kCurrentDirectoryString[] = "./";
#endif
static bool IsPathSeparator(char c) {
#if GTEST_HAS_ALT_PATH_SEP_
  return (c == kPathSeparator) || (c == kAlternatePathSeparator);
#else
  return c == kPathSeparator;
#endif
}
FilePath FilePath::GetCurrentDir() {
#if GTEST_OS_WINDOWS_MOBILE
  return FilePath(kCurrentDirectoryString);
#elif GTEST_OS_WINDOWS
  char cwd[GTEST_PATH_MAX_ + 1] = { '\0' };
  return FilePath(_getcwd(cwd, sizeof(cwd)) == NULL ? "" : cwd);
#else
  char cwd[GTEST_PATH_MAX_ + 1] = { '\0' };
  return FilePath(getcwd(cwd, sizeof(cwd)) == NULL ? "" : cwd);
#endif
}
FilePath FilePath::RemoveExtension(const char* extension) const {
  const std::string dot_extension = std::string(".") + extension;
  if (String::EndsWithCaseInsensitive(pathname_, dot_extension)) {
    return FilePath(pathname_.substr(
        0, pathname_.length() - dot_extension.length()));
  }
  return *this;
}
const char* FilePath::FindLastPathSeparator() const {
  const char* const last_sep = strrchr(c_str(), kPathSeparator);
#if GTEST_HAS_ALT_PATH_SEP_
  const char* const last_alt_sep = strrchr(c_str(), kAlternatePathSeparator);
  if (last_alt_sep != NULL &&
      (last_sep == NULL || last_alt_sep > last_sep)) {
    return last_alt_sep;
  }
#endif
  return last_sep;
}
FilePath FilePath::RemoveDirectoryName() const {
  const char* const last_sep = FindLastPathSeparator();
  return last_sep ? FilePath(last_sep + 1) : *this;
}
FilePath FilePath::RemoveFileName() const {
  const char* const last_sep = FindLastPathSeparator();
  std::string dir;
  if (last_sep) {
    dir = std::string(c_str(), last_sep + 1 - c_str());
  } else {
    dir = kCurrentDirectoryString;
  }
  return FilePath(dir);
}
FilePath FilePath::MakeFileName(const FilePath& directory,
                                const FilePath& base_name,
                                int number,
                                const char* extension) {
  std::string file;
  if (number == 0) {
    file = base_name.string() + "." + extension;
  } else {
    file = base_name.string() + "_" + StreamableToString(number)
        + "." + extension;
  }
  return ConcatPaths(directory, FilePath(file));
}
FilePath FilePath::ConcatPaths(const FilePath& directory,
                               const FilePath& relative_path) {
  if (directory.IsEmpty())
    return relative_path;
  const FilePath dir(directory.RemoveTrailingPathSeparator());
  return FilePath(dir.string() + kPathSeparator + relative_path.string());
}
bool FilePath::FileOrDirectoryExists() const {
#if GTEST_OS_WINDOWS_MOBILE
  LPCWSTR unicode = String::AnsiToUtf16(pathname_.c_str());
  const DWORD attributes = GetFileAttributes(unicode);
  delete [] unicode;
  return attributes != kInvalidFileAttributes;
#else
  posix::StatStruct file_stat;
  return posix::Stat(pathname_.c_str(), &file_stat) == 0;
#endif
}
bool FilePath::DirectoryExists() const {
  bool result = false;
#if GTEST_OS_WINDOWS
  const FilePath& path(IsRootDirectory() ? *this :
                                           RemoveTrailingPathSeparator());
#else
  const FilePath& path(*this);
#endif
#if GTEST_OS_WINDOWS_MOBILE
  LPCWSTR unicode = String::AnsiToUtf16(path.c_str());
  const DWORD attributes = GetFileAttributes(unicode);
  delete [] unicode;
  if ((attributes != kInvalidFileAttributes) &&
      (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
    result = true;
  }
#else
  posix::StatStruct file_stat;
  result = posix::Stat(path.c_str(), &file_stat) == 0 &&
      posix::IsDir(file_stat);
#endif
  return result;
}
bool FilePath::IsRootDirectory() const {
#if GTEST_OS_WINDOWS
  return pathname_.length() == 3 && IsAbsolutePath();
#else
  return pathname_.length() == 1 && IsPathSeparator(pathname_.c_str()[0]);
#endif
}
bool FilePath::IsAbsolutePath() const {
  const char* const name = pathname_.c_str();
#if GTEST_OS_WINDOWS
  return pathname_.length() >= 3 &&
     ((name[0] >= 'a' && name[0] <= 'z') ||
      (name[0] >= 'A' && name[0] <= 'Z')) &&
     name[1] == ':' &&
     IsPathSeparator(name[2]);
#else
  return IsPathSeparator(name[0]);
#endif
}
FilePath FilePath::GenerateUniqueFileName(const FilePath& directory,
                                          const FilePath& base_name,
                                          const char* extension) {
  FilePath full_pathname;
  int number = 0;
  do {
    full_pathname.Set(MakeFileName(directory, base_name, number++, extension));
  } while (full_pathname.FileOrDirectoryExists());
  return full_pathname;
}
bool FilePath::IsDirectory() const {
  return !pathname_.empty() &&
         IsPathSeparator(pathname_.c_str()[pathname_.length() - 1]);
}
bool FilePath::CreateDirectoriesRecursively() const {
  if (!this->IsDirectory()) {
    return false;
  }
  if (pathname_.length() == 0 || this->DirectoryExists()) {
    return true;
  }
  const FilePath parent(this->RemoveTrailingPathSeparator().RemoveFileName());
  return parent.CreateDirectoriesRecursively() && this->CreateFolder();
}
bool FilePath::CreateFolder() const {
#if GTEST_OS_WINDOWS_MOBILE
  FilePath removed_sep(this->RemoveTrailingPathSeparator());
  LPCWSTR unicode = String::AnsiToUtf16(removed_sep.c_str());
  int result = CreateDirectory(unicode, NULL) ? 0 : -1;
  delete [] unicode;
#elif GTEST_OS_WINDOWS
  int result = _mkdir(pathname_.c_str());
#else
  int result = mkdir(pathname_.c_str(), 0777);
#endif
  if (result == -1) {
    return this->DirectoryExists();
  }
  return true;
}
FilePath FilePath::RemoveTrailingPathSeparator() const {
  return IsDirectory()
      ? FilePath(pathname_.substr(0, pathname_.length() - 1))
      : *this;
}
void FilePath::Normalize() {
  if (pathname_.c_str() == NULL) {
    pathname_ = "";
    return;
  }
  const char* src = pathname_.c_str();
  char* const dest = new char[pathname_.length() + 1];
  char* dest_ptr = dest;
  memset(dest_ptr, 0, pathname_.length() + 1);
  while (*src != '\0') {
    *dest_ptr = *src;
    if (!IsPathSeparator(*src)) {
      src++;
    } else {
#if GTEST_HAS_ALT_PATH_SEP_
      if (*dest_ptr == kAlternatePathSeparator) {
        *dest_ptr = kPathSeparator;
      }
#endif
      while (IsPathSeparator(*src))
        src++;
    }
    dest_ptr++;
  }
  *dest_ptr = '\0';
  pathname_ = dest;
  delete[] dest;
}
}
}
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if GTEST_OS_WINDOWS_MOBILE
# include <windows.h>
#elif GTEST_OS_WINDOWS
# include <io.h>
# include <sys/stat.h>
#else
# include <unistd.h>
#endif
#if GTEST_OS_MAC
# include <mach/mach_init.h>
# include <mach/task.h>
# include <mach/vm_map.h>
#endif
#if GTEST_OS_QNX
# include <devctl.h>
# include <sys/procfs.h>
#endif
#define GTEST_IMPLEMENTATION_ 1
#undef GTEST_IMPLEMENTATION_
namespace testing {
namespace internal {
#if defined(_MSC_VER) || defined(__BORLANDC__)
const int kStdOutFileno = 1;
const int kStdErrFileno = 2;
#else
const int kStdOutFileno = STDOUT_FILENO;
const int kStdErrFileno = STDERR_FILENO;
#endif
#if GTEST_OS_MAC
size_t GetThreadCount() {
  const task_t task = mach_task_self();
  mach_msg_type_number_t thread_count;
  thread_act_array_t thread_list;
  const kern_return_t status = task_threads(task, &thread_list, &thread_count);
  if (status == KERN_SUCCESS) {
    vm_deallocate(task,
                  reinterpret_cast<vm_address_t>(thread_list),
                  sizeof(thread_t) * thread_count);
    return static_cast<size_t>(thread_count);
  } else {
    return 0;
  }
}
#elif GTEST_OS_QNX
size_t GetThreadCount() {
  const int fd = open("/proc/self/as", O_RDONLY);
  if (fd < 0) {
    return 0;
  }
  procfs_info process_info;
  const int status =
      devctl(fd, DCMD_PROC_INFO, &process_info, sizeof(process_info), NULL);
  close(fd);
  if (status == EOK) {
    return static_cast<size_t>(process_info.num_threads);
  } else {
    return 0;
  }
}
#else
size_t GetThreadCount() {
  return 0;
}
#endif
#if GTEST_USES_POSIX_RE
RE::~RE() {
  if (is_valid_) {
    regfree(&partial_regex_);
    regfree(&full_regex_);
  }
  free(const_cast<char*>(pattern_));
}
bool RE::FullMatch(const char* str, const RE& re) {
  if (!re.is_valid_) return false;
  regmatch_t match;
  return regexec(&re.full_regex_, str, 1, &match, 0) == 0;
}
bool RE::PartialMatch(const char* str, const RE& re) {
  if (!re.is_valid_) return false;
  regmatch_t match;
  return regexec(&re.partial_regex_, str, 1, &match, 0) == 0;
}
void RE::Init(const char* regex) {
  pattern_ = posix::StrDup(regex);
  const size_t full_regex_len = strlen(regex) + 10;
  char* const full_pattern = new char[full_regex_len];
  snprintf(full_pattern, full_regex_len, "^(%s)$", regex);
  is_valid_ = regcomp(&full_regex_, full_pattern, REG_EXTENDED) == 0;
  if (is_valid_) {
    const char* const partial_regex = (*regex == '\0') ? "()" : regex;
    is_valid_ = regcomp(&partial_regex_, partial_regex, REG_EXTENDED) == 0;
  }
  EXPECT_TRUE(is_valid_)
      << "Regular expression \"" << regex
      << "\" is not a valid POSIX Extended regular expression.";
  delete[] full_pattern;
}
#elif GTEST_USES_SIMPLE_RE
bool IsInSet(char ch, const char* str) {
  return ch != '\0' && strchr(str, ch) != NULL;
}
bool IsAsciiDigit(char ch) { return '0' <= ch && ch <= '9'; }
bool IsAsciiPunct(char ch) {
  return IsInSet(ch, "^-!\"#$%&'()*+,./:;<=>?@[\\]_`{|}~");
}
bool IsRepeat(char ch) { return IsInSet(ch, "?*+"); }
bool IsAsciiWhiteSpace(char ch) { return IsInSet(ch, " \f\n\r\t\v"); }
bool IsAsciiWordChar(char ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') ||
      ('0' <= ch && ch <= '9') || ch == '_';
}
bool IsValidEscape(char c) {
  return (IsAsciiPunct(c) || IsInSet(c, "dDfnrsStvwW"));
}
bool AtomMatchesChar(bool escaped, char pattern_char, char ch) {
  if (escaped) {
    switch (pattern_char) {
      case 'd': return IsAsciiDigit(ch);
      case 'D': return !IsAsciiDigit(ch);
      case 'f': return ch == '\f';
      case 'n': return ch == '\n';
      case 'r': return ch == '\r';
      case 's': return IsAsciiWhiteSpace(ch);
      case 'S': return !IsAsciiWhiteSpace(ch);
      case 't': return ch == '\t';
      case 'v': return ch == '\v';
      case 'w': return IsAsciiWordChar(ch);
      case 'W': return !IsAsciiWordChar(ch);
    }
    return IsAsciiPunct(pattern_char) && pattern_char == ch;
  }
  return (pattern_char == '.' && ch != '\n') || pattern_char == ch;
}
std::string FormatRegexSyntaxError(const char* regex, int index) {
  return (Message() << "Syntax error at index " << index
          << " in simple regular expression \"" << regex << "\": ").GetString();
}
bool ValidateRegex(const char* regex) {
  if (regex == NULL) {
    ADD_FAILURE() << "NULL is not a valid simple regular expression.";
    return false;
  }
  bool is_valid = true;
  bool prev_repeatable = false;
  for (int i = 0; regex[i]; i++) {
    if (regex[i] == '\\') {
      i++;
      if (regex[i] == '\0') {
        ADD_FAILURE() << FormatRegexSyntaxError(regex, i - 1)
                      << "'\\' cannot appear at the end.";
        return false;
      }
      if (!IsValidEscape(regex[i])) {
        ADD_FAILURE() << FormatRegexSyntaxError(regex, i - 1)
                      << "invalid escape sequence \"\\" << regex[i] << "\".";
        is_valid = false;
      }
      prev_repeatable = true;
    } else {
      const char ch = regex[i];
      if (ch == '^' && i > 0) {
        ADD_FAILURE() << FormatRegexSyntaxError(regex, i)
                      << "'^' can only appear at the beginning.";
        is_valid = false;
      } else if (ch == '$' && regex[i + 1] != '\0') {
        ADD_FAILURE() << FormatRegexSyntaxError(regex, i)
                      << "'$' can only appear at the end.";
        is_valid = false;
      } else if (IsInSet(ch, "()[]{}|")) {
        ADD_FAILURE() << FormatRegexSyntaxError(regex, i)
                      << "'" << ch << "' is unsupported.";
        is_valid = false;
      } else if (IsRepeat(ch) && !prev_repeatable) {
        ADD_FAILURE() << FormatRegexSyntaxError(regex, i)
                      << "'" << ch << "' can only follow a repeatable token.";
        is_valid = false;
      }
      prev_repeatable = !IsInSet(ch, "^$?*+");
    }
  }
  return is_valid;
}
bool MatchRepetitionAndRegexAtHead(
    bool escaped, char c, char repeat, const char* regex,
    const char* str) {
  const size_t min_count = (repeat == '+') ? 1 : 0;
  const size_t max_count = (repeat == '?') ? 1 :
      static_cast<size_t>(-1) - 1;
  for (size_t i = 0; i <= max_count; ++i) {
    if (i >= min_count && MatchRegexAtHead(regex, str + i)) {
      return true;
    }
    if (str[i] == '\0' || !AtomMatchesChar(escaped, c, str[i]))
      return false;
  }
  return false;
}
bool MatchRegexAtHead(const char* regex, const char* str) {
  if (*regex == '\0')
    return true;
  if (*regex == '$')
    return *str == '\0';
  const bool escaped = *regex == '\\';
  if (escaped)
    ++regex;
  if (IsRepeat(regex[1])) {
    return MatchRepetitionAndRegexAtHead(
        escaped, regex[0], regex[1], regex + 2, str);
  } else {
    return (*str != '\0') && AtomMatchesChar(escaped, *regex, *str) &&
        MatchRegexAtHead(regex + 1, str + 1);
  }
}
bool MatchRegexAnywhere(const char* regex, const char* str) {
  if (regex == NULL || str == NULL)
    return false;
  if (*regex == '^')
    return MatchRegexAtHead(regex + 1, str);
  do {
    if (MatchRegexAtHead(regex, str))
      return true;
  } while (*str++ != '\0');
  return false;
}
RE::~RE() {
  free(const_cast<char*>(pattern_));
  free(const_cast<char*>(full_pattern_));
}
bool RE::FullMatch(const char* str, const RE& re) {
  return re.is_valid_ && MatchRegexAnywhere(re.full_pattern_, str);
}
bool RE::PartialMatch(const char* str, const RE& re) {
  return re.is_valid_ && MatchRegexAnywhere(re.pattern_, str);
}
void RE::Init(const char* regex) {
  pattern_ = full_pattern_ = NULL;
  if (regex != NULL) {
    pattern_ = posix::StrDup(regex);
  }
  is_valid_ = ValidateRegex(regex);
  if (!is_valid_) {
    return;
  }
  const size_t len = strlen(regex);
  char* buffer = static_cast<char*>(malloc(len + 3));
  full_pattern_ = buffer;
  if (*regex != '^')
    *buffer++ = '^';
  memcpy(buffer, regex, len);
  buffer += len;
  if (len == 0 || regex[len - 1] != '$')
    *buffer++ = '$';
  *buffer = '\0';
}
#endif
const char kUnknownFile[] = "unknown file";
GTEST_API_ ::std::string FormatFileLocation(const char* file, int line) {
  const std::string file_name(file == NULL ? kUnknownFile : file);
  if (line < 0) {
    return file_name + ":";
  }
#ifdef _MSC_VER
  return file_name + "(" + StreamableToString(line) + "):";
#else
  return file_name + ":" + StreamableToString(line) + ":";
#endif
}
GTEST_API_ ::std::string FormatCompilerIndependentFileLocation(
    const char* file, int line) {
  const std::string file_name(file == NULL ? kUnknownFile : file);
  if (line < 0)
    return file_name;
  else
    return file_name + ":" + StreamableToString(line);
}
GTestLog::GTestLog(GTestLogSeverity severity, const char* file, int line)
    : severity_(severity) {
  const char* const marker =
      severity == GTEST_INFO ? "[  INFO ]" :
      severity == GTEST_WARNING ? "[WARNING]" :
      severity == GTEST_ERROR ? "[ ERROR ]" : "[ FATAL ]";
  GetStream() << ::std::endl << marker << " "
              << FormatFileLocation(file, line).c_str() << ": ";
}
GTestLog::~GTestLog() {
  GetStream() << ::std::endl;
  if (severity_ == GTEST_FATAL) {
    fflush(stderr);
    posix::Abort();
  }
}
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
#if GTEST_HAS_STREAM_REDIRECTION
class CapturedStream {
 public:
  explicit CapturedStream(int fd) : fd_(fd), uncaptured_fd_(dup(fd)) {
# if GTEST_OS_WINDOWS
    char temp_dir_path[MAX_PATH + 1] = { '\0' };
    char temp_file_path[MAX_PATH + 1] = { '\0' };
    ::GetTempPathA(sizeof(temp_dir_path), temp_dir_path);
    const UINT success = ::GetTempFileNameA(temp_dir_path,
                                            "gtest_redir",
                                            0,
                                            temp_file_path);
    GTEST_CHECK_(success != 0)
        << "Unable to create a temporary file in " << temp_dir_path;
    const int captured_fd = creat(temp_file_path, _S_IREAD | _S_IWRITE);
    GTEST_CHECK_(captured_fd != -1) << "Unable to open temporary file "
                                    << temp_file_path;
    filename_ = temp_file_path;
# else
# if GTEST_OS_LINUX_ANDROID
    char name_template[] = "/sdcard/gtest_captured_stream.XXXXXX";
# else
    char name_template[] = "/tmp/captured_stream.XXXXXX";
# endif
    const int captured_fd = mkstemp(name_template);
    filename_ = name_template;
# endif
    fflush(NULL);
    dup2(captured_fd, fd_);
    close(captured_fd);
  }
  ~CapturedStream() {
    remove(filename_.c_str());
  }
  std::string GetCapturedString() {
    if (uncaptured_fd_ != -1) {
      fflush(NULL);
      dup2(uncaptured_fd_, fd_);
      close(uncaptured_fd_);
      uncaptured_fd_ = -1;
    }
    FILE* const file = posix::FOpen(filename_.c_str(), "r");
    const std::string content = ReadEntireFile(file);
    posix::FClose(file);
    return content;
  }
 private:
  static std::string ReadEntireFile(FILE* file);
  static size_t GetFileSize(FILE* file);
  const int fd_;
  int uncaptured_fd_;
  ::std::string filename_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(CapturedStream);
};
size_t CapturedStream::GetFileSize(FILE* file) {
  fseek(file, 0, SEEK_END);
  return static_cast<size_t>(ftell(file));
}
std::string CapturedStream::ReadEntireFile(FILE* file) {
  const size_t file_size = GetFileSize(file);
  char* const buffer = new char[file_size];
  size_t bytes_last_read = 0;
  size_t bytes_read = 0;
  fseek(file, 0, SEEK_SET);
  do {
    bytes_last_read = fread(buffer+bytes_read, 1, file_size-bytes_read, file);
    bytes_read += bytes_last_read;
  } while (bytes_last_read > 0 && bytes_read < file_size);
  const std::string content(buffer, bytes_read);
  delete[] buffer;
  return content;
}
# ifdef _MSC_VER
#pragma warning(pop)
# endif
static CapturedStream* g_captured_stderr = NULL;
static CapturedStream* g_captured_stdout = NULL;
void CaptureStream(int fd, const char* stream_name, CapturedStream** stream) {
  if (*stream != NULL) {
    GTEST_LOG_(FATAL) << "Only one " << stream_name
                      << " capturer can exist at a time.";
  }
  *stream = new CapturedStream(fd);
}
std::string GetCapturedStream(CapturedStream** captured_stream) {
  const std::string content = (*captured_stream)->GetCapturedString();
  delete *captured_stream;
  *captured_stream = NULL;
  return content;
}
void CaptureStdout() {
  CaptureStream(kStdOutFileno, "stdout", &g_captured_stdout);
}
void CaptureStderr() {
  CaptureStream(kStdErrFileno, "stderr", &g_captured_stderr);
}
std::string GetCapturedStdout() {
  return GetCapturedStream(&g_captured_stdout);
}
std::string GetCapturedStderr() {
  return GetCapturedStream(&g_captured_stderr);
}
#endif
#if GTEST_HAS_DEATH_TEST
::std::vector<testing::internal::string> g_argvs;
static const ::std::vector<testing::internal::string>* g_injected_test_argvs =
                                        NULL;
void SetInjectableArgvs(const ::std::vector<testing::internal::string>* argvs) {
  if (g_injected_test_argvs != argvs)
    delete g_injected_test_argvs;
  g_injected_test_argvs = argvs;
}
const ::std::vector<testing::internal::string>& GetInjectableArgvs() {
  if (g_injected_test_argvs != NULL) {
    return *g_injected_test_argvs;
  }
  return g_argvs;
}
#endif
#if GTEST_OS_WINDOWS_MOBILE
namespace posix {
void Abort() {
  DebugBreak();
  TerminateProcess(GetCurrentProcess(), 1);
}
}
#endif
static std::string FlagToEnvVar(const char* flag) {
  const std::string full_flag =
      (Message() << GTEST_FLAG_PREFIX_ << flag).GetString();
  Message env_var;
  for (size_t i = 0; i != full_flag.length(); i++) {
    env_var << ToUpper(full_flag.c_str()[i]);
  }
  return env_var.GetString();
}
bool ParseInt32(const Message& src_text, const char* str, Int32* value) {
  char* end = NULL;
  const long long_value = strtol(str, &end, 10);
  if (*end != '\0') {
    Message msg;
    msg << "WARNING: " << src_text
        << " is expected to be a 32-bit integer, but actually"
        << " has value \"" << str << "\".\n";
    printf("%s", msg.GetString().c_str());
    fflush(stdout);
    return false;
  }
  const Int32 result = static_cast<Int32>(long_value);
  if (long_value == LONG_MAX || long_value == LONG_MIN ||
      result != long_value
      ) {
    Message msg;
    msg << "WARNING: " << src_text
        << " is expected to be a 32-bit integer, but actually"
        << " has value " << str << ", which overflows.\n";
    printf("%s", msg.GetString().c_str());
    fflush(stdout);
    return false;
  }
  *value = result;
  return true;
}
bool BoolFromGTestEnv(const char* flag, bool default_value) {
  const std::string env_var = FlagToEnvVar(flag);
  const char* const string_value = posix::GetEnv(env_var.c_str());
  return string_value == NULL ?
      default_value : strcmp(string_value, "0") != 0;
}
Int32 Int32FromGTestEnv(const char* flag, Int32 default_value) {
  const std::string env_var = FlagToEnvVar(flag);
  const char* const string_value = posix::GetEnv(env_var.c_str());
  if (string_value == NULL) {
    return default_value;
  }
  Int32 result = default_value;
  if (!ParseInt32(Message() << "Environment variable " << env_var,
                  string_value, &result)) {
    printf("The default value %s is used.\n",
           (Message() << default_value).GetString().c_str());
    fflush(stdout);
    return default_value;
  }
  return result;
}
const char* StringFromGTestEnv(const char* flag, const char* default_value) {
  const std::string env_var = FlagToEnvVar(flag);
  const char* const value = posix::GetEnv(env_var.c_str());
  return value == NULL ? default_value : value;
}
}
}
#include <ctype.h>
#include <stdio.h>
#include <ostream>
#include <string>
namespace testing {
namespace {
using ::std::ostream;
void PrintByteSegmentInObjectTo(const unsigned char* obj_bytes, size_t start,
                                size_t count, ostream* os) {
  char text[5] = "";
  for (size_t i = 0; i != count; i++) {
    const size_t j = start + i;
    if (i != 0) {
      if ((j % 2) == 0)
        *os << ' ';
      else
        *os << '-';
    }
    GTEST_SNPRINTF_(text, sizeof(text), "%02X", obj_bytes[j]);
    *os << text;
  }
}
void PrintBytesInObjectToImpl(const unsigned char* obj_bytes, size_t count,
                              ostream* os) {
  *os << count << "-byte object <";
  const size_t kThreshold = 132;
  const size_t kChunkSize = 64;
  if (count < kThreshold) {
    PrintByteSegmentInObjectTo(obj_bytes, 0, count, os);
  } else {
    PrintByteSegmentInObjectTo(obj_bytes, 0, kChunkSize, os);
    *os << " ... ";
    const size_t resume_pos = (count - kChunkSize + 1)/2*2;
    PrintByteSegmentInObjectTo(obj_bytes, resume_pos, count - resume_pos, os);
  }
  *os << ">";
}
}
namespace internal2 {
void PrintBytesInObjectTo(const unsigned char* obj_bytes, size_t count,
                          ostream* os) {
  PrintBytesInObjectToImpl(obj_bytes, count, os);
}
}
namespace internal {
enum CharFormat {
  kAsIs,
  kHexEscape,
  kSpecialEscape
};
inline bool IsPrintableAscii(wchar_t c) {
  return 0x20 <= c && c <= 0x7E;
}
template <typename UnsignedChar, typename Char>
static CharFormat PrintAsCharLiteralTo(Char c, ostream* os) {
  switch (static_cast<wchar_t>(c)) {
    case L'\0':
      *os << "\\0";
      break;
    case L'\'':
      *os << "\\'";
      break;
    case L'\\':
      *os << "\\\\";
      break;
    case L'\a':
      *os << "\\a";
      break;
    case L'\b':
      *os << "\\b";
      break;
    case L'\f':
      *os << "\\f";
      break;
    case L'\n':
      *os << "\\n";
      break;
    case L'\r':
      *os << "\\r";
      break;
    case L'\t':
      *os << "\\t";
      break;
    case L'\v':
      *os << "\\v";
      break;
    default:
      if (IsPrintableAscii(c)) {
        *os << static_cast<char>(c);
        return kAsIs;
      } else {
        *os << "\\x" + String::FormatHexInt(static_cast<UnsignedChar>(c));
        return kHexEscape;
      }
  }
  return kSpecialEscape;
}
static CharFormat PrintAsStringLiteralTo(wchar_t c, ostream* os) {
  switch (c) {
    case L'\'':
      *os << "'";
      return kAsIs;
    case L'"':
      *os << "\\\"";
      return kSpecialEscape;
    default:
      return PrintAsCharLiteralTo<wchar_t>(c, os);
  }
}
static CharFormat PrintAsStringLiteralTo(char c, ostream* os) {
  return PrintAsStringLiteralTo(
      static_cast<wchar_t>(static_cast<unsigned char>(c)), os);
}
template <typename UnsignedChar, typename Char>
void PrintCharAndCodeTo(Char c, ostream* os) {
  *os << ((sizeof(c) > 1) ? "L'" : "'");
  const CharFormat format = PrintAsCharLiteralTo<UnsignedChar>(c, os);
  *os << "'";
  if (c == 0)
    return;
  *os << " (" << static_cast<int>(c);
  if (format == kHexEscape || (1 <= c && c <= 9)) {
  } else {
    *os << ", 0x" << String::FormatHexInt(static_cast<UnsignedChar>(c));
  }
  *os << ")";
}
void PrintTo(unsigned char c, ::std::ostream* os) {
  PrintCharAndCodeTo<unsigned char>(c, os);
}
void PrintTo(signed char c, ::std::ostream* os) {
  PrintCharAndCodeTo<unsigned char>(c, os);
}
void PrintTo(wchar_t wc, ostream* os) {
  PrintCharAndCodeTo<wchar_t>(wc, os);
}
template <typename CharType>
static void PrintCharsAsStringTo(
    const CharType* begin, size_t len, ostream* os) {
  const char* const kQuoteBegin = sizeof(CharType) == 1 ? "\"" : "L\"";
  *os << kQuoteBegin;
  bool is_previous_hex = false;
  for (size_t index = 0; index < len; ++index) {
    const CharType cur = begin[index];
    if (is_previous_hex && IsXDigit(cur)) {
      *os << "\" " << kQuoteBegin;
    }
    is_previous_hex = PrintAsStringLiteralTo(cur, os) == kHexEscape;
  }
  *os << "\"";
}
template <typename CharType>
static void UniversalPrintCharArray(
    const CharType* begin, size_t len, ostream* os) {
  if (len > 0 && begin[len - 1] == '\0') {
    PrintCharsAsStringTo(begin, len - 1, os);
    return;
  }
  PrintCharsAsStringTo(begin, len, os);
  *os << " (no terminating NUL)";
}
void UniversalPrintArray(const char* begin, size_t len, ostream* os) {
  UniversalPrintCharArray(begin, len, os);
}
void UniversalPrintArray(const wchar_t* begin, size_t len, ostream* os) {
  UniversalPrintCharArray(begin, len, os);
}
void PrintTo(const char* s, ostream* os) {
  if (s == NULL) {
    *os << "NULL";
  } else {
    *os << ImplicitCast_<const void*>(s) << " pointing to ";
    PrintCharsAsStringTo(s, strlen(s), os);
  }
}
#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
void PrintTo(const wchar_t* s, ostream* os) {
  if (s == NULL) {
    *os << "NULL";
  } else {
    *os << ImplicitCast_<const void*>(s) << " pointing to ";
    PrintCharsAsStringTo(s, wcslen(s), os);
  }
}
#endif
#if GTEST_HAS_GLOBAL_STRING
void PrintStringTo(const ::string& s, ostream* os) {
  PrintCharsAsStringTo(s.data(), s.size(), os);
}
#endif
void PrintStringTo(const ::std::string& s, ostream* os) {
  PrintCharsAsStringTo(s.data(), s.size(), os);
}
#if GTEST_HAS_GLOBAL_WSTRING
void PrintWideStringTo(const ::wstring& s, ostream* os) {
  PrintCharsAsStringTo(s.data(), s.size(), os);
}
#endif
#if GTEST_HAS_STD_WSTRING
void PrintWideStringTo(const ::std::wstring& s, ostream* os) {
  PrintCharsAsStringTo(s.data(), s.size(), os);
}
#endif
}
}
#define GTEST_IMPLEMENTATION_ 1
#undef GTEST_IMPLEMENTATION_
namespace testing {
using internal::GetUnitTestImpl;
std::string TestPartResult::ExtractSummary(const char* message) {
  const char* const stack_trace = strstr(message, internal::kStackTraceMarker);
  return stack_trace == NULL ? message :
      std::string(message, stack_trace);
}
std::ostream& operator<<(std::ostream& os, const TestPartResult& result) {
  return os
      << result.file_name() << ":" << result.line_number() << ": "
      << (result.type() == TestPartResult::kSuccess ? "Success" :
          result.type() == TestPartResult::kFatalFailure ? "Fatal failure" :
          "Non-fatal failure") << ":\n"
      << result.message() << std::endl;
}
void TestPartResultArray::Append(const TestPartResult& result) {
  array_.push_back(result);
}
const TestPartResult& TestPartResultArray::GetTestPartResult(int index) const {
  if (index < 0 || index >= size()) {
    printf("\nInvalid index (%d) into TestPartResultArray.\n", index);
    internal::posix::Abort();
  }
  return array_[index];
}
int TestPartResultArray::size() const {
  return static_cast<int>(array_.size());
}
namespace internal {
HasNewFatalFailureHelper::HasNewFatalFailureHelper()
    : has_new_fatal_failure_(false),
      original_reporter_(GetUnitTestImpl()->
                         GetTestPartResultReporterForCurrentThread()) {
  GetUnitTestImpl()->SetTestPartResultReporterForCurrentThread(this);
}
HasNewFatalFailureHelper::~HasNewFatalFailureHelper() {
  GetUnitTestImpl()->SetTestPartResultReporterForCurrentThread(
      original_reporter_);
}
void HasNewFatalFailureHelper::ReportTestPartResult(
    const TestPartResult& result) {
  if (result.fatally_failed())
    has_new_fatal_failure_ = true;
  original_reporter_->ReportTestPartResult(result);
}
}
}
namespace testing {
namespace internal {
#if GTEST_HAS_TYPED_TEST_P
static const char* SkipSpaces(const char* str) {
  while (IsSpace(*str))
    str++;
  return str;
}
const char* TypedTestCasePState::VerifyRegisteredTestNames(
    const char* file, int line, const char* registered_tests) {
  typedef ::std::set<const char*>::const_iterator DefinedTestIter;
  registered_ = true;
  registered_tests = SkipSpaces(registered_tests);
  Message errors;
  ::std::set<std::string> tests;
  for (const char* names = registered_tests; names != NULL;
       names = SkipComma(names)) {
    const std::string name = GetPrefixUntilComma(names);
    if (tests.count(name) != 0) {
      errors << "Test " << name << " is listed more than once.\n";
      continue;
    }
    bool found = false;
    for (DefinedTestIter it = defined_test_names_.begin();
         it != defined_test_names_.end();
         ++it) {
      if (name == *it) {
        found = true;
        break;
      }
    }
    if (found) {
      tests.insert(name);
    } else {
      errors << "No test named " << name
             << " can be found in this test case.\n";
    }
  }
  for (DefinedTestIter it = defined_test_names_.begin();
       it != defined_test_names_.end();
       ++it) {
    if (tests.count(*it) == 0) {
      errors << "You forgot to list test " << *it << ".\n";
    }
  }
  const std::string& errors_str = errors.GetString();
  if (errors_str != "") {
    fprintf(stderr, "%s %s", FormatFileLocation(file, line).c_str(),
            errors_str.c_str());
    fflush(stderr);
    posix::Abort();
  }
  return registered_tests;
}
#endif
}
}
