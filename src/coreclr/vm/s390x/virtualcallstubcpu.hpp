// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// VirtualCallStubCpu.hpp
//
#ifndef _VIRTUAL_CALL_STUB_S390X_H
#define _VIRTUAL_CALL_STUB_S390X_H

#define USES_LOOKUP_STUBS   1

struct LookupStub
{
    inline PCODE entryPoint() { LIMITED_METHOD_CONTRACT; return (PCODE)&_entryPoint[0]; }
    inline size_t token() { LIMITED_METHOD_CONTRACT; return _token; }
    inline size_t size() { LIMITED_METHOD_CONTRACT; return sizeof(LookupStub); }
private :
    friend struct LookupHolder;

    DWORD _entryPoint[4];
    PCODE _resolveWorkerTarget;
    size_t _token;
};

struct LookupHolder
{
private:
    LookupStub _stub;
public:
    static void InitializeStatic() { }

    void  Initialize(LookupHolder* pLookupHolderRX, PCODE resolveWorkerTarget, size_t dispatchToken)
    {
        // FIXME
        memset (_stub._entryPoint, 0, sizeof (_stub._entryPoint));
        _stub._entryPoint[0] = 0x101;

        _stub._resolveWorkerTarget = resolveWorkerTarget;
        _stub._token               = dispatchToken;
    }

    LookupStub*    stub()        { LIMITED_METHOD_CONTRACT; return &_stub; }
    static LookupHolder*  FromLookupEntry(PCODE lookupEntry)
    {
        return (LookupHolder*) ( lookupEntry - offsetof(LookupHolder, _stub) - offsetof(LookupStub, _entryPoint)  );
    }
};

struct DispatchStub
{
    inline PCODE entryPoint()         { LIMITED_METHOD_CONTRACT; return (PCODE)&_entryPoint[0]; }

    inline size_t expectedMT()  { LIMITED_METHOD_CONTRACT; return _expectedMT; }
    inline PCODE implTarget()  { LIMITED_METHOD_CONTRACT; return _implTarget; }

    inline TADDR implTargetSlot(EntryPointSlots::SlotType *slotTypeRef) const
    {
        _ASSERTE(!"S390X:NYI");
        return NULL;
    }

    inline PCODE failTarget()  { LIMITED_METHOD_CONTRACT; return _failTarget; }
    inline size_t size()        { LIMITED_METHOD_CONTRACT; return sizeof(DispatchStub); }

private:
    friend struct DispatchHolder;

    DWORD _entryPoint[8];
    size_t  _expectedMT;
    PCODE _implTarget;
    PCODE _failTarget;
};

struct DispatchHolder
{
    static void InitializeStatic() { }

    void Initialize(DispatchHolder* pDispatchHolderRX, PCODE implTarget, PCODE failTarget, size_t expectedMT)
    {
        _ASSERTE(!"S390X:NYI");
    }

    DispatchStub* stub()      { LIMITED_METHOD_CONTRACT; return &_stub; }

    static DispatchHolder*  FromDispatchEntry(PCODE dispatchEntry)
    {
        _ASSERTE(!"S390X:NYI");
        return NULL;
    }

private:
    DispatchStub _stub;
};

struct ResolveStub
{
    inline PCODE failEntryPoint()            { LIMITED_METHOD_CONTRACT; return (PCODE)&_failEntryPoint[0]; }
    inline PCODE resolveEntryPoint()         { LIMITED_METHOD_CONTRACT; return (PCODE)&_resolveEntryPoint[0]; }
    inline PCODE slowEntryPoint()            { LIMITED_METHOD_CONTRACT; return (PCODE)&_slowEntryPoint[0]; }
    inline size_t  token()                   { LIMITED_METHOD_CONTRACT; return _token; }
    inline INT32*  pCounter()                { LIMITED_METHOD_CONTRACT; return _pCounter; }

    inline UINT32  hashedToken()             { LIMITED_METHOD_CONTRACT; return _hashedToken >> LOG2_PTRSIZE;    }
    inline size_t  cacheAddress()            { LIMITED_METHOD_CONTRACT; return _cacheAddress;   }
    inline size_t  size()                    { LIMITED_METHOD_CONTRACT; return sizeof(ResolveStub); }

private:
    friend struct ResolveHolder;
    const static int resolveEntryPointLen = 17;
    const static int slowEntryPointLen = 4;
    const static int failEntryPointLen = 8;

    DWORD _resolveEntryPoint[resolveEntryPointLen];
    DWORD _slowEntryPoint[slowEntryPointLen];
    DWORD _failEntryPoint[failEntryPointLen];
    INT32*  _pCounter;               //Base of the Data Region
    size_t  _cacheAddress;           // lookupCache
    size_t  _token;
    PCODE   _resolveWorkerTarget;
    UINT32  _hashedToken;
};

struct ResolveHolder
{
    static void  InitializeStatic() { }

    void Initialize(ResolveHolder* pResolveHolderRX,
                    PCODE resolveWorkerTarget, PCODE patcherTarget,
                    size_t dispatchToken, UINT32 hashedToken,
                    void * cacheAddr, INT32 * counterAddr)
    {
        _ASSERTE(!"S390X:NYI");
    }

    ResolveStub* stub()      { LIMITED_METHOD_CONTRACT; return &_stub; }

    static ResolveHolder*  FromFailEntry(PCODE failEntry);
    static ResolveHolder*  FromResolveEntry(PCODE resolveEntry);
private:
    ResolveStub _stub;
};


/*VTableCallStub**************************************************************************************
These are jump stubs that perform a vtable-base virtual call. These stubs assume that an object is placed
in the first argument register (this pointer). From there, the stub extracts the MethodTable pointer, followed by the
vtable pointer, and finally jumps to the target method at a given slot in the vtable.
*/
struct VTableCallStub
{
    friend struct VTableCallHolder;

    inline size_t size()
    {
        _ASSERTE(!"S390X:NYI");
        return 0;
    }

    inline PCODE        entryPoint()        const { LIMITED_METHOD_CONTRACT;  return (PCODE)&_entryPoint[0]; }

    inline size_t token()
    {
        _ASSERTE(!"S390X:NYI");
        return 0;
    }

private:
    BYTE    _entryPoint[0];         // Dynamically sized stub. See Initialize() for more details.
};

/* VTableCallHolders are the containers for VTableCallStubs, they provide for any alignment of
stubs as necessary.  */
struct VTableCallHolder
{
    void  Initialize(unsigned slot);

    VTableCallStub* stub() { LIMITED_METHOD_CONTRACT;  return reinterpret_cast<VTableCallStub *>(this); }

    static size_t GetHolderSize(unsigned slot)
    {
        _ASSERTE(!"S390X:NYI");
        return 0;
    }

    static VTableCallHolder* FromVTableCallEntry(PCODE entry) { LIMITED_METHOD_CONTRACT; return (VTableCallHolder*)entry; }

private:
    // VTableCallStub follows here. It is dynamically sized on allocation because it could
    // use short/long instruction sizes for LDR, depending on the slot value.
};


#ifdef DECLARE_DATA

#ifndef DACCESS_COMPILE
ResolveHolder* ResolveHolder::FromFailEntry(PCODE failEntry)
{
    _ASSERTE(!"S390X:NYI");
    return NULL;
}

ResolveHolder* ResolveHolder::FromResolveEntry(PCODE resolveEntry)
{
    _ASSERTE(!"S390X:NYI");
    return NULL;
}

void VTableCallHolder::Initialize(unsigned slot)
{
    _ASSERTE(!"S390X:NYI");
}

#endif // DACCESS_COMPILE

#endif //DECLARE_DATA

#endif // _VIRTUAL_CALL_STUB_S390X_H
