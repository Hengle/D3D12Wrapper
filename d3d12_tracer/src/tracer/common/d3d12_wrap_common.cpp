//
// Created by ZZK on 2024/7/17.
//

#include <tracer/common/d3d12_wrap_common.h>
#include <tracer/core/wrapper_creators.h>
#include <tracer/common/logger.h>

#pragma comment(lib, "comsupp.lib")

	using namespace gfxshim;
	using namespace gfxshim::encode;
    // IUnknownWrapper
    IUnknownWrapper::IUnknownWrapper(const IID &riid, IUnknown *wrapper_object)
    : m_riid(riid), m_object(wrapper_object, false), m_ref_count(1)
    {

    }

    HRESULT STDMETHODCALLTYPE IUnknownWrapper::QueryInterface(REFIID riid, void** object)
    {
		// TODO: check wrap
		IID target_interface_id = riid;
		const auto result = m_object->QueryInterface(target_interface_id, object);
		if (SUCCEEDED(result) && IsEqualIID(target_interface_id, __uuidof(IUnknown)))
		{
			*object = this;
			D3D12_WRAPPER_DEBUG("Invoke IUnknownWrapper::QueryInterface change");
		} else
		{
			encode::WrapObject(riid, object);
		}
		D3D12_WRAPPER_DEBUG("Invoke IUnknownWrapper::QueryInterface no chang");
		return result;
    }

    ULONG STDMETHODCALLTYPE IUnknownWrapper::AddRef()
    {
        const auto result = ++m_ref_count;
        return result;
    }

    ULONG STDMETHODCALLTYPE IUnknownWrapper::Release()
    {
        const auto result = --m_ref_count;
        return result;
    }

    REFIID IUnknownWrapper::GetRiid() const
    {
        return m_riid;
    }

    IUnknown* IUnknownWrapper::GetWrappedObject()
    {
        return m_object;
    }

    const IUnknown* IUnknownWrapper::GetWrappedObject() const
    {
        return m_object;
    }

    uint32_t IUnknownWrapper::GetRefCount() const
    {
        return m_ref_count.load(std::memory_order_seq_cst);
    }

    IUnknownWrapper::~IUnknownWrapper()
    {
        m_object = nullptr;
        m_ref_count.store(0, std::memory_order_release);
    }

namespace gfxshim::encode {
	void UnwrapStructObjects(std::vector<D3D12_STATE_SUBOBJECT> &unwrapped_sub_objects, std::span<const D3D12_STATE_SUBOBJECT> wrapped_sub_objects)
	{
		for (size_t i = 0U; i < unwrapped_sub_objects.size(); ++i)
		{
			UnwrapStructObjects(unwrapped_sub_objects[i], unwrapped_sub_objects, wrapped_sub_objects);
		}
	}

	void UnwrapStructObjects(D3D12_STATE_SUBOBJECT &unwrapped_sub_object, std::vector<D3D12_STATE_SUBOBJECT> &unwrapped_sub_objects, std::span<const D3D12_STATE_SUBOBJECT> wrapped_sub_objects)
	{
		if (unwrapped_sub_object.pDesc != nullptr)
		{
			switch (unwrapped_sub_object.Type)
			{
				case D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE:
				{
					auto global_root_signature = static_cast<D3D12_GLOBAL_ROOT_SIGNATURE *>(const_cast<void *>(unwrapped_sub_object.pDesc));
					global_root_signature->pGlobalRootSignature = GetWrappedObject<ID3D12RootSignature>(global_root_signature->pGlobalRootSignature);
					break;
				}
				case D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE:
				{
					auto local_root_signature = static_cast<D3D12_LOCAL_ROOT_SIGNATURE *>(const_cast<void *>(unwrapped_sub_object.pDesc));
					local_root_signature->pLocalRootSignature = GetWrappedObject<ID3D12RootSignature>(local_root_signature->pLocalRootSignature);
					break;
				}
				case D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION:
				{
					auto existing_collection_desc = static_cast<D3D12_EXISTING_COLLECTION_DESC *>(const_cast<void *>(unwrapped_sub_object.pDesc));
					existing_collection_desc->pExistingCollection = GetWrappedObject<ID3D12StateObject>(existing_collection_desc->pExistingCollection);
					break;
				}
				case D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION:
				{
					auto exports_association = static_cast<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION *>(const_cast<void *>(unwrapped_sub_object.pDesc));
					UnwrapStructObjects(*exports_association, unwrapped_sub_objects, wrapped_sub_objects);
					break;
				}
				default:
					break;
			}
		}
	}

	void UnwrapStructObjects(D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION &unwrapped_exports_association, std::vector<D3D12_STATE_SUBOBJECT> &unwrapped_sub_objects, std::span<const D3D12_STATE_SUBOBJECT> wrapped_sub_objects)
	{
		// TODO: may exist a pointer to an existing sub-object structure, must check it
		if (!wrapped_sub_objects.empty() && unwrapped_exports_association.pSubobjectToAssociate != nullptr)
		{
			for (size_t i = 0U; i < wrapped_sub_objects.size(); ++i)
			{
				if (unwrapped_exports_association.pSubobjectToAssociate == &wrapped_sub_objects[i])
				{
					unwrapped_exports_association.pSubobjectToAssociate = &unwrapped_sub_objects[i];
					return;
				}
			}
		}
		if (unwrapped_exports_association.pSubobjectToAssociate != nullptr)
		{
			UnwrapStructObjects(*const_cast<D3D12_STATE_SUBOBJECT *>(unwrapped_exports_association.pSubobjectToAssociate), unwrapped_sub_objects, wrapped_sub_objects);
		}
	}

    }










