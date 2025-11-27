#pragma once

#include "../WinApi/WinHeaders.h"
#include <cassert>
#include <optional>

namespace MemX {
	template <typename T>
	class NtResult {
		private:
		NTSTATUS status = STATUS_UNSUCCESSFUL;
		std::optional<T> result_data = std::nullopt;

		public:
		NtResult() = default;
		NtResult(T result_, NTSTATUS status_ = STATUS_SUCCESS)
			: status(status_), result_data(std::move(result_)) {
			assert(result_data.has_value());
		}
		NtResult(NTSTATUS status) : status(status), result_data(std::nullopt) {
			assert(!NT_SUCCESS(status));
		}

		bool success() const { return NT_SUCCESS(status); }
		T& result() { return result_data.value(); }
		const T& result() const { return result_data.value(); }
		T result(const T& default_value) {
			return result_data.value_or(default_value);
		}

		explicit operator bool() const { return NT_SUCCESS(status); }
		explicit operator T() const { return result_data.value(); }
		T* operator->() { return &result_data.value(); }
		T& operator*() { return result_data.value(); }
	};
} // namespace MemX

