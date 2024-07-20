//
// Created by ZZK on 2024/7/20.
//

#pragma once

#include "d3d12_device5_wrap.h"

namespace gfxshim
{
    class ID3D12Device6Wrapper : public ID3D12Device5Wrapper
    {
    public:
        ID3D12Device6Wrapper(REFIID riid, IUnknown *object);

        ~ID3D12Device6Wrapper() override;

        virtual HRESULT STDMETHODCALLTYPE SetBackgroundProcessingMode(
                D3D12_BACKGROUND_PROCESSING_MODE Mode,
                D3D12_MEASUREMENTS_ACTION MeasurementsAction,
                HANDLE hEventToSignalUponCompletion,
                BOOL* pbFurtherMeasurementsDesired);
    };
}
