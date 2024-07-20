//
// Created by ZZK on 2024/7/17.
//

#pragma once

#include <d3d12.h>
#include <d3dcommon.h>
#include <guiddef.h>
#include <windef.h>
#include <minwinbase.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <dxgicommon.h>
#include <dxgiformat.h>
#include <dxgitype.h>
#include <atomic>

namespace gfxshim
{
    const IID IID_IUnknown_Wrapper = { 0xe00bb2cc, 0x162e, 0x4aad, { 0x97, 0x69, 0xed, 0xe6, 0x91, 0x53, 0x95, 0xf6 } };

    MIDL_INTERFACE("E00BB2CC-162E-4AAD-9769-EDE6915395F6")
    IUnknownWrapper : public IUnknown
    {
    public:
        IUnknownWrapper(REFIID riid, IUnknown *wrapper_object);

        virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override;

        virtual ULONG STDMETHODCALLTYPE AddRef() override;

        virtual ULONG STDMETHODCALLTYPE Release() override;

        REFIID GetRiid() const;

        IUnknown* GetWrappedObject();

        const IUnknown* GetWrappedObject() const;

        template <typename T>
        T *GetWrappedObjectAs()
        {
            return reinterpret_cast<T *>(m_object);
        }

        uint32_t GetRefCount() const;

        virtual ~IUnknownWrapper();

    protected:
        IID m_riid;
        IUnknown *m_object;
        std::atomic_uint32_t m_ref_count;
    };

    class ID3D12ObjectWrapper : public IUnknownWrapper
    {
    public:
        ID3D12ObjectWrapper(REFIID riid, IUnknown *object);

        virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
                _In_  REFGUID guid,
                _Inout_  UINT* pDataSize,
                _Out_writes_bytes_opt_(*pDataSize)  void* pData);

        virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
                _In_  REFGUID guid,
                _In_  UINT DataSize,
                _In_reads_bytes_opt_(DataSize)  const void* pData);

        virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
                _In_  REFGUID guid,
                _In_opt_  const IUnknown* pData);

        virtual HRESULT STDMETHODCALLTYPE SetName(
                _In_z_  LPCWSTR Name);
    };

    class ID3D12DeviceChildWrapper : public ID3D12ObjectWrapper
    {
    public:
        ID3D12DeviceChildWrapper(REFIID riid, IUnknown *object);

        virtual HRESULT STDMETHODCALLTYPE GetDevice(
                REFIID riid,
                void** ppvDevice);
    };

    class ID3D12PageableWrapper : public ID3D12DeviceChildWrapper
    {
    public:
        ID3D12PageableWrapper(REFIID riid, IUnknown *object);
    };

    namespace encode
    {
        template <typename T>
        T *GetWrappedObject(T *wrapped_object)
        {
            if (wrapped_object != nullptr)
            {
                return reinterpret_cast<IUnknownWrapper *>(wrapped_object)->GetWrappedObjectAs<T>();
            }
            return nullptr;
        }

        template <typename T>
        const T *GetWrappedObject(const T *wrapped_object)
        {
            if (wrapped_object != nullptr)
            {
                return reinterpret_cast<const IUnknownWrapper *>(wrapped_object)->GetWrappedObjectAs<T>();
            }
            return nullptr;
        }
    }
}
















