# MemX

MemX is mainly used to interact with and manipulate processes on Windows. 
At present, it provides the function of attaching to the running process, reading and writing memory

## How to Use MemX

Here's a basic example demonstrating how to attach to a process, read a value from its memory, and then write a new value.

```cpp
#include <iostream>
#include <MemX/Process/Process.h>
#include <MemX/Common/WinApi/NtResult.h>

int main() {
    MemX::Process process;
    DWORD targetPid = 0x1234; // Replace with the actual PID of your target process
    
    // Attach to the process by PID
    NTSTATUS status = process.Catch(targetPid);

    if (NT_SUCCESS(status)) {
        std::wcout << L"Successfully attached to process with PID: " << targetPid << std::endl;

        // Check if the process is active
        if (process.Core().isActive()) {
            // Example memory address (replace with an actual address from your target process)
            MemX::ptr_t memoryAddress = 0x600B4130; 

            // Read a DWORD from memory
            MemX::NtResult<DWORD> readResult = process.Memory().Read<DWORD>(memoryAddress);
            if (readResult.success()) {
                std::cout << "Read value at " << std::hex << memoryAddress << ": " << std::dec << readResult.result() << std::endl;
            } else {
                std::wcerr << L"Failed to read memory. NTSTATUS: " << std::hex << readResult.status() << std::endl;
            }

            // Write a new DWORD value to memory
            DWORD newValue = 123456789;
            NTSTATUS writeStatus = process.Memory().Write<DWORD>(memoryAddress, newValue);
            if (NT_SUCCESS(writeStatus)) {
                std::cout << "Successfully wrote " << newValue << " to " << std::hex << memoryAddress << std::endl;
            } else {
                std::wcerr << L"Failed to write memory. NTSTATUS: " << std::hex << writeStatus << std::endl;
            }

            // Verify the write by reading again
            readResult = process.Memory().Read<DWORD>(memoryAddress);
            if (readResult.success()) {
                std::cout << "Verified new value: " << std::dec << readResult.result() << std::endl;
            }
        } else {
            std::wcerr << L"Process is not active." << std::endl;
        }

        // Don't forget to drop the process handle when done
        process.Drop();
    } else {
        std::wcerr << L"Failed to attach to process. NTSTATUS: " << std::hex << status << std::endl;
    }

    return 0;
}
```

### Attaching to a process

You can attach to a process in several ways:

*   **By Process ID (PID):**
    ```cpp
    DWORD pid = GetCurrentProcessId(); // Example: get current process PID
    NTSTATUS status = process.Catch(pid);
    ```
*   **By Process Name:**
    ```cpp
    const wchar_t* processName = L"notepad.exe"; 
    NTSTATUS status = process.Catch(processName);
    ```
*   **By Process Handle:**
    ```cpp
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetPid);
    if (hProcess) {
        NTSTATUS status = process.Catch(hProcess);
        CloseHandle(hProcess); // MemX takes ownership/duplicates the handle, so you can close yours
    }
    ```

### Memory Reading and Writing

#### Basic Read/Write

```cpp
// Read a 4-byte integer (DWORD)
MemX::ptr_t address = 0x12345678;
MemX::NtResult<DWORD> value = process.Memory().Read<DWORD>(address);
if (value.success()) {
    std::cout << "Value: " << value.result() << std::endl;
}

// Write a 4-byte integer (DWORD)
DWORD newValue = 987654321;
NTSTATUS writeStatus = process.Memory().Write<DWORD>(address, newValue);
if (NT_SUCCESS(writeStatus)) {
    // Success
}
```

#### Reading/Writing Pointer Chains

If you need to read a value that is at an offset from a base address found through a series of pointers:

```cpp
// Example: BaseAddress -> Pointer1 (offset +0x10) -> Value (offset +0x20)
std::vector<MemX::ptr_t> pointerChain = {
    0xFEEDFACE, // Base address of the first pointer
    0x10,       // Offset to the second pointer
    0x20        // Offset to the final value
};

MemX::NtResult<int> finalValue = process.Memory().Read<int>(pointerChain);
if (finalValue.success()) {
    std::cout << "Chained Value: " << finalValue.result() << std::endl;
}

int newChainedValue = 42;
NTSTATUS chainedWriteStatus = process.Memory().Write<int>(pointerChain, newChainedValue);
```

## Error Handling

MemX functions often return `NTSTATUS` codes or `MemX::NtResult<T>`.

*   **`NTSTATUS`:** Check `NT_SUCCESS(status)` or `NT_FAILED(status)` macros.
*   **`MemX::NtResult<T>`:** This is a wrapper containing the result (`.result()`) and the `NTSTATUS` (`.status()`). Use `.success()` or `.failed()` methods to check the operation's outcome.
