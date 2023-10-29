section .data
    format_file_open_failed db "Failed to open file", 0
    format_invalid_file_format db "Error: Invalid file format", 0
    format_memory_allocation_failed db "Memory allocation failed", 0
    format_numStates_numTransitions db "%d,%d", 0
    format_startState db "%d", 0
    format_state_id db "%d,", 0
    format_transition db "%d,%d,%c", 0

section .text
global readDfa

readDfa:
    ; Open the file
    push ebp
    mov ebp, esp
    sub esp, 8
    mov eax, [ebp + 8] ; filename
    push eax
    call fopen
    add esp, 4
    mov ebx, eax ; store the file pointer in ebx

    cmp ebx, 0
    je file_open_failed

    ; Read the number of states and transitions from the file
    push dword format_numStates_numTransitions
    push dword [esi + DFA.numStates]
    push ebx
    call fscanf
    add esp, 12

    cmp eax, 2
    jne invalid_file_format

    ; Allocate memory for the DFA structure
    push dword sizeof_DFA
    call malloc
    add esp, 4
    mov esi, eax ; store the DFA pointer in esi

    cmp esi, 0
    je memory_allocation_failed

    ; Read the state IDs from the file
    mov ecx, dword [esi + DFA.numStates]
    mov edi, dword [esi + DFA.states]
    mov edx, ebx
    mov eax, 0

    read_state_ids:
    push dword format_state_id
    push edi
    push edx
    call fscanf
    add esp, 12

    cmp eax, 1
    jne invalid_file_format

    inc edi
    loop read_state_ids

    ; Read the accepting state IDs from the file
    push dword format_state_id
    push edi
    push edx
    call fscanf
    add esp, 12

    cmp eax, 1
    jne invalid_file_format

    ; Read the start state from the file
    push dword format_startState
    push dword [esi + DFA.startState]
    push edx
    call fscanf
    add esp, 12

    cmp eax, 1
    jne invalid_file_format

    ; Allocate memory for the transitions
    mov ecx, dword [esi + DFA.numTransitions]
    mov edi, dword [esi + DFA.transitions]
    mov edx, ebx
    mov eax, 0

    push dword sizeof_Transition
    imul ecx, eax
    call malloc
    add esp, 4
    mov edi, eax ; store the transitions pointer in edi

    cmp edi, 0
    je memory_allocation_failed

    ; Read the transitions from the file
    mov ecx, dword [esi + DFA.numTransitions]
    mov edi, dword [esi + DFA.transitions]
    mov edx, ebx
    mov eax, 0

    read_transitions:
    push dword format_transition
    push edi
    push edx
    call fscanf
    add esp, 12

    cmp eax, 3
    jne invalid_file_format

    inc edi
    loop read_transitions

    ; Close the file
    push ebx
    call fclose
    add esp, 4

    ; Return the DFA pointer
    mov eax, esi
    jmp cleanup

file_open_failed:
    push dword format_file_open_failed
    call printf
    add esp, 4
    mov eax, 0 ; return NULL
    jmp cleanup

invalid_file_format:
    push dword format_invalid_file_format
    call printf
    add esp, 4
    mov eax, 0 ; return NULL
    jmp cleanup

memory_allocation_failed:
    push dword format_memory_allocation_failed
    call printf
    add esp, 4
    mov eax, 0 ; return NULL
    jmp cleanup

cleanup:
    mov esp, ebp
    pop ebp
    ret
