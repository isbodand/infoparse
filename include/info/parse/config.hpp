//
// Created by bodand on 2019-08-07.
//

#pragma once

#define INFO_USE_BOOST

namespace info::parse::config {
  // Retries callback function if deemed failed by
  // return value
#ifdef INFO_RETRY_FAILED_CALLBACK_FUNCTION
  static constexpr bool RetryFailedCallback = true;
#else
  static constexpr bool RetryFailedCallback = false;
#endif

  // Option_ doesn't thrown exception if callback is not "well-formed"
  // for library use
#ifdef INFO_PARSER_FAIL_BAD_FUNCTION_SILENTLY
  static constexpr bool FailSilently = true;
#else
  static constexpr bool FailSilently = false;
#endif

  // Deletes the returned pointer of the callback function
#ifdef INFO_DELETE_RETURN_VALUE_OF_CALLBACK
  static constexpr bool DeleteCallbackReturn = true;
#else
  static constexpr bool DeleteCallbackReturn = false;
#endif

  // Build time in library version
#if !defined(INFO_USE_BUILD_TIME_IN_VERSION) \
    || INFO_USE_BUILD_TIME_IN_VERSION == 1
  static constexpr bool UseBuildTimeInVersion = true;
#else
  static constexpr bool UseBuildTimeInVersion = false;
#endif
}