#pragma once
#include "base/task/BinarySemaphore.h"
#include "base/task/Mutex.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "memory_dma_handle.h"
#include <cstddef>
#include <cstdint>

namespace bsp
{
	class MemoryDma1 final :
		public base::memory_dma::memory_dma_handle
	{
	private:
		class handle_context
		{
		public:
			handle_context(MemoryDma1 *self)
			{
				_self = self;
			}

			DMA_HandleTypeDef _handle{};
			MemoryDma1 *_self{};
		};

		base::UsageStateManager<MemoryDma1> _usage_state_manager{};
		handle_context _handle_context{this};
		base::task::Mutex _lock{};
		base::task::BinarySemaphore _complete_signal{false};
		size_t _align = 4;
		bool _is_error = false;
		bool _is_abort = false;

		void InitializeCallback();
		void InitializeInterrupt();

		void OnCompleteCallback()
		{
			_complete_signal.ReleaseFromIsr();
		}

		void OnErrorCallback()
		{
			_is_error = true;
			_complete_signal.ReleaseFromIsr();
		}

		void OnAbortCallback()
		{
			_is_abort = true;
			_complete_signal.ReleaseFromIsr();
		}

	public:
		~MemoryDma1();

		virtual void Initialize() override;

		virtual void Initialize(size_t align) override;

		virtual void Copy(uint8_t const *begin, uint8_t const *end, uint8_t *dst) override;
	};

} // namespace bsp
