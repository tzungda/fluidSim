//---faceCenteredGrid3.cpp
#include "faceCenteredGrid3.h"

void faceCenteredGrid3::resetSampler()
{
    LinearBufferSampler uSampler(
        &mDataU, gridSpacing(), mDataOriginU);
    LinearBufferSampler vSampler(
        &mDataV, gridSpacing(), mDataOriginV);
    LinearBufferSampler wSampler(
        &mDataW, gridSpacing(), mDataOriginW);

    mLinearSamplerU = uSampler;
    mLinearSamplerV = vSampler;
    mLinearSamplerW = wSampler;

    mSampler = [uSampler, vSampler, wSampler](const vector3& x) -> vector3 {
        FloatType u = uSampler(x);
        FloatType v = vSampler(x);
        FloatType w = wSampler(x);
        return vector3(u, v, w);
    };
}

faceCenteredGrid3::faceCenteredGrid3():mDataOriginU(0.0, 0.5, 0.5),
mDataOriginV(0.5, 0.0, 0.5),
mDataOriginW(0.5, 0.5, 0.0),
mLinearSamplerU(
    LinearBufferSampler(
        &mDataU,
        vector3(1.0, 1.0, 1.0),
        mDataOriginU)),
    mLinearSamplerV(
        LinearBufferSampler(
            &mDataV,
            vector3(1.0, 1.0, 1.0),
            mDataOriginV)),
    mLinearSamplerW(
        LinearBufferSampler(
            &mDataW,
            vector3(1.0, 1.0, 1.0),
            mDataOriginW))
{
}

faceCenteredGrid3::faceCenteredGrid3(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& origin,
    const vector3& initialValue):
    mLinearSamplerU(
        LinearBufferSampler(
            &mDataU,
            vector3(1.0, 1.0, 1.0),
            mDataOriginU)),
    mLinearSamplerV(
        LinearBufferSampler(
            &mDataV,
            vector3(1.0, 1.0, 1.0),
            mDataOriginV)),
    mLinearSamplerW(
        LinearBufferSampler(
            &mDataW,
            vector3(1.0, 1.0, 1.0),
            mDataOriginW))
{
    resize(resolution, gridSpacing, origin, initialValue);
}

faceCenteredGrid3::faceCenteredGrid3( const faceCenteredGrid3& other ):
    mLinearSamplerU(
        LinearBufferSampler(
            &mDataU,
            vector3(1.0, 1.0, 1.0),
            mDataOriginU)),
    mLinearSamplerV(
        LinearBufferSampler(
            &mDataV,
            vector3(1.0, 1.0, 1.0),
            mDataOriginV)),
    mLinearSamplerW(
        LinearBufferSampler(
            &mDataW,
            vector3(1.0, 1.0, 1.0),
            mDataOriginW))
{
    set( other );
}

faceCenteredGrid3::~faceCenteredGrid3()
{
}

void faceCenteredGrid3::set( const faceCenteredGrid3 &other )
{
    setGrid(other);

    mDataU.set(other.mDataU);
    mDataV.set(other.mDataV);
    mDataW.set(other.mDataW);
    mDataOriginU = other.mDataOriginU;
    mDataOriginV = other.mDataOriginV;
    mDataOriginW = other.mDataOriginW;

    resetSampler();
}

FloatType &faceCenteredGrid3::u( size_t i, size_t j, size_t k )
{
    return mDataU(i, j, k);
}

FloatType &faceCenteredGrid3::v( size_t i, size_t j, size_t k )
{
    return mDataV(i, j, k);
}

FloatType &faceCenteredGrid3::w( size_t i, size_t j, size_t k )
{
    return mDataW(i, j, k);
}

const FloatType &faceCenteredGrid3::u( size_t i, size_t j, size_t k ) const
{
    return mDataU(i, j, k);
}

const FloatType &faceCenteredGrid3::v( size_t i, size_t j, size_t k ) const
{
    return mDataV(i, j, k);
}

const FloatType &faceCenteredGrid3::w( size_t i, size_t j, size_t k ) const
{
    return mDataW(i, j, k);
}

FloatType &faceCenteredGrid3::u( const size3& pt )
{
    return mDataU( pt );
}

FloatType &faceCenteredGrid3::v( const size3& pt )
{
    return mDataV( pt );
}

FloatType &faceCenteredGrid3::w( const size3& pt )
{
    return mDataW( pt );
}

const FloatType &faceCenteredGrid3::u( const size3& pt ) const
{
    return mDataU( pt );
}

const FloatType &faceCenteredGrid3::v( const size3& pt ) const
{
    return mDataV( pt );
}

const FloatType &faceCenteredGrid3::w( const size3& pt ) const
{
    return mDataW( pt );
}

vector3 faceCenteredGrid3::uPosition( size_t i, size_t j, size_t k ) const
{
    vector3 h = gridSpacing();
    return mDataOriginU + h * vector3({(FloatType)i, (FloatType)j, (FloatType)k});
}

vector3 faceCenteredGrid3::vPosition( size_t i, size_t j, size_t k ) const
{
    vector3 h = gridSpacing();
    return mDataOriginV + h * vector3({(FloatType)i, (FloatType)j, (FloatType)k});
}

vector3 faceCenteredGrid3::wPosition( size_t i, size_t j, size_t k ) const
{
    vector3 h = gridSpacing();
    return mDataOriginW + h * vector3({(FloatType)i, (FloatType)j, (FloatType)k});
}

vectorGrid3::DataPositionFunc faceCenteredGrid3::uPosition() const
{
    vector3 h = gridSpacing();
    return [this, h](size_t i, size_t j, size_t k) -> vector3 {
        return mDataOriginU + h * vector3({(FloatType)i, (FloatType)j, (FloatType)k});
    };
}

vectorGrid3::DataPositionFunc faceCenteredGrid3::vPosition() const
{
    vector3 h = gridSpacing();
    return [this, h](size_t i, size_t j, size_t k) -> vector3 {
        return mDataOriginV + h * vector3({(FloatType)i, (FloatType)j, (FloatType)k});
    };
}

vectorGrid3::DataPositionFunc faceCenteredGrid3::wPosition() const
{
    vector3 h = gridSpacing();
    return [this, h](size_t i, size_t j, size_t k) -> vector3 {
        return mDataOriginW + h * vector3({(FloatType)i, (FloatType)j, (FloatType)k});
    };
}

size3 faceCenteredGrid3::uSize() const
{
    return mDataU.size();
}

size3 faceCenteredGrid3::vSize() const
{
    return mDataV.size();
}

size3 faceCenteredGrid3::wSize() const
{
    return mDataW.size();
}

dataBuffer3& faceCenteredGrid3::uData()
{
    return mDataU;
}

const dataBuffer3& faceCenteredGrid3::uData() const
{
    return mDataU;
}

dataBuffer3& faceCenteredGrid3::vData()
{
    return mDataV;
}

const dataBuffer3& faceCenteredGrid3::vData() const
{
    return mDataV;
}

dataBuffer3& faceCenteredGrid3::wData()
{
    return mDataW;
}

const dataBuffer3& faceCenteredGrid3::wData() const
{
    return mDataW;
}

dataBuffer3& faceCenteredGrid3::dataByIndex( int i )
{
    switch( i )
    {
    case 0:
    {
        return mDataU;
    }
    case 1:
    {
        return mDataV;
    }
    default:
    {
        break;
    }
    }
    return mDataW;
}

const dataBuffer3& faceCenteredGrid3::dataByIndex( int i ) const
{
    switch( i )
    {
    case 0:
    {
        return mDataU;
    }
    case 1:
    {
        return mDataV;
    }
    default:
    {
        break;
    }
    }
    return mDataW;
}

void faceCenteredGrid3::forEachUIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mDataU.forEachIndex(func);
}

void faceCenteredGrid3::forEachVIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mDataV.forEachIndex(func);
}

void faceCenteredGrid3::forEachWIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mDataW.forEachIndex(func);
}

#ifdef _OPENMP
void faceCenteredGrid3::forEachUIndexOpenMP(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mDataU.forEachIndexOpenMP(func);
}

void faceCenteredGrid3::forEachVIndexOpenMP(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mDataV.forEachIndexOpenMP(func);
}

void faceCenteredGrid3::forEachWIndexOpenMP(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mDataW.forEachIndexOpenMP(func);
}
#endif

FloatType faceCenteredGrid3::divergenceAtCellCenterByIndex( size_t i, size_t j, size_t k ) const
{
    const vector3& gs = gridSpacing();
    FloatType leftU = mDataU( i, j, k );
    FloatType rightU = mDataU( i + 1, j, k );
    FloatType downV = mDataV( i, j, k );
    FloatType upV = mDataV( i, j + 1, k );
    FloatType backW = mDataW( i, j, k );
    FloatType frontW = mDataW( i, j, k + 1);

    return ( rightU - leftU ) / gs.x + ( upV - downV )/gs.y + (frontW - backW)/gs.z;
}

vector3 faceCenteredGrid3::valueAtCellCenterByIndex( size_t i, size_t j, size_t k ) const
{
    return 0.5 * vector3( mDataU( i, j, k ) + mDataU( i + 1, j, k ),
        mDataV( i, j, k ) + mDataV( i, j + 1, k ),
        mDataW( i, j, k ) + mDataW( i, j, k + 1 ) );
}

vector3 faceCenteredGrid3::curlAtCellCenterByIndex( size_t i, size_t j, size_t k ) const
{
    const size3 s = resolution();
    const vector3& gs = gridSpacing();

    vector3 left = valueAtCellCenterByIndex( (i > 0)? i - 1: i, j, k );
    vector3 right = valueAtCellCenterByIndex( (i + 1 < s.x)? i + 1: i, j, k );
    vector3 down = valueAtCellCenterByIndex( i, (j > 0)? j - 1: j, k );
    vector3 up = valueAtCellCenterByIndex( i, (j +1 > s.y)? j + 1: j, k );
    vector3 back = valueAtCellCenterByIndex( i, j, (k > 0)? k - 1: k );
    vector3 front = valueAtCellCenterByIndex( i, j, (k < s.z)? k + 1: k );

    FloatType fx_y_lower = down.x;
    FloatType fx_y_upper = up.x;
    FloatType fx_z_lower = back.x;
    FloatType fx_z_upper = front.x;

    FloatType fy_x_lower = left.y;
    FloatType fy_x_upper = right.y;
    FloatType fy_z_lower = back.y;
    FloatType fy_z_upper = front.y;

    FloatType fz_x_lower = left.z;
    FloatType fz_x_upper = right.z;
    FloatType fz_y_lower = down.z;
    FloatType fz_y_upper = up.z;

    return vector3( (FloatType)0.5 * ( fz_y_upper - fx_y_lower )/gs.y - (FloatType)0.5 * ( fy_z_upper - fy_z_lower )/gs.z,
        (FloatType)0.5 * ( fx_z_upper - fx_z_lower )/gs.z - (FloatType)0.5 * ( fz_x_upper - fz_x_lower )/gs.x,
        (FloatType)0.5 * ( fy_x_upper - fy_x_lower )/gs.x - (FloatType)0.5 * ( fx_y_upper - fx_y_lower )/gs.y );
}

void faceCenteredGrid3::fillZero()
{
    mDataU.setZero();
    mDataV.setZero();
    mDataW.setZero();
}

void faceCenteredGrid3::onResize( const size3& resolution, const vector3& gridSpacing, const vector3& origin, const vector3& initValue )
{
    mDataU.resize( resolution + size3( 1, 0, 0 ), initValue.x );
    mDataV.resize( resolution + size3( 0, 1, 0 ), initValue.y );
    mDataW.resize( resolution + size3( 0, 0, 1 ), initValue.z );

    mDataOriginU = origin + 0.5 * vector3( 0.0, gridSpacing.y, gridSpacing.z );
    mDataOriginV = origin + 0.5 * vector3( gridSpacing.x, 0.0, gridSpacing.z );
    mDataOriginW = origin + 0.5 * vector3( gridSpacing.x, gridSpacing.y, 0.0 );

    resetSampler();
}

std::shared_ptr<vectorGrid3> faceCenteredGrid3::clone() const
{
    return std::shared_ptr<faceCenteredGrid3>(
        new faceCenteredGrid3(*this),
        [] (faceCenteredGrid3* obj) {
        delete obj;
    });
}

vector3 faceCenteredGrid3::sample(const vector3& x) const
{
    return mSampler( x );
}

void faceCenteredGrid3::fill(const vector3& value)
{
    //
    for ( size_t i = 0; i < mDataU.width(); ++i )
    {
        for ( size_t j = 0; j < mDataU.height(); ++j )
        {
            for ( size_t k = 0; k < mDataU.depth(); ++k )
            {
                mDataU(i, j, k) = value.x;
            }
        }
    }
    //
    for ( size_t i = 0; i < mDataV.width(); ++i )
    {
        for ( size_t j = 0; j < mDataV.height(); ++j )
        {
            for ( size_t k = 0; k < mDataV.depth(); ++k )
            {
                mDataV(i, j, k) = value.y;
            }
        }
    }
    //
    for ( size_t i = 0; i < mDataW.width(); ++i )
    {
        for ( size_t j = 0; j < mDataW.height(); ++j )
        {
            for ( size_t k = 0; k < mDataW.depth(); ++k )
            {
                mDataW(i, j, k) = value.z;
            }
        }
    }
}

void faceCenteredGrid3::fill(const std::function<vector3(const vector3&)>& func)
{
    DataPositionFunc uPos = uPosition();
    for ( size_t i = 0; i < mDataU.width(); ++i )
    {
        for ( size_t j = 0; j < mDataU.height(); ++j )
        {
            for ( size_t k = 0; k < mDataU.depth(); ++k )
            {
                mDataU(i, j, k) = func(uPos(i, j, k)).x;
            }
        }
    }

    DataPositionFunc vPos = vPosition();
    for ( size_t i = 0; i < mDataV.width(); ++i )
    {
        for ( size_t j = 0; j < mDataV.height(); ++j )
        {
            for ( size_t k = 0; k < mDataV.depth(); ++k )
            {
                mDataV(i, j, k) = func(vPos(i, j, k)).y;
            }
        }
    }

    DataPositionFunc wPos = wPosition();
    for ( size_t i = 0; i < mDataW.width(); ++i )
    {
        for ( size_t j = 0; j < mDataW.height(); ++j )
        {
            for ( size_t k = 0; k < mDataW.depth(); ++k )
            {
                mDataW(i, j, k) = func(wPos(i, j, k)).z;
            }
        }
    }

}

void faceCenteredGrid3::swap(grid3* other)
{
    faceCenteredGrid3* sameType
        = dynamic_cast<faceCenteredGrid3*>(other);

    if (sameType != nullptr)
    {
        swapGrid(sameType);

        mDataU.swap(sameType->mDataU);
        mDataV.swap(sameType->mDataV);
        mDataW.swap(sameType->mDataW);
        std::swap(mDataOriginU, sameType->mDataOriginU);
        std::swap(mDataOriginV, sameType->mDataOriginV);
        std::swap(mDataOriginW, sameType->mDataOriginW);
        std::swap(mLinearSamplerU, sameType->mLinearSamplerU);
        std::swap(mLinearSamplerV, sameType->mLinearSamplerV);
        std::swap(mLinearSamplerW, sameType->mLinearSamplerW);
        std::swap(mSampler, sameType->mSampler);
    }
}

void faceCenteredGrid3::getData(std::vector<FloatType>* data) const
{
    size_t size
        = uSize().x * uSize().y * uSize().z
        + vSize().x * vSize().y * vSize().z
        + wSize().x * wSize().y * wSize().z;
    data->resize(size);
    size_t cnt = 0;
    mDataU.forEach([&] (FloatType value) {
        (*data)[cnt++] = value;
    });
    mDataV.forEach([&] (FloatType value) {
        (*data)[cnt++] = value;
    });
    mDataW.forEach([&] (FloatType value) {
        (*data)[cnt++] = value;
    });
}


void faceCenteredGrid3::setData(const std::vector<FloatType>& data)
{
    size_t cnt = 0;
    mDataU.forEachIndex([&] (size_t i, size_t j, size_t k) {
        mDataU(i, j, k) = data[cnt++];
    });
    mDataV.forEachIndex([&] (size_t i, size_t j, size_t k) {
        mDataV(i, j, k) = data[cnt++];
    });
    mDataW.forEachIndex([&] (size_t i, size_t j, size_t k) {
        mDataW(i, j, k) = data[cnt++];
    });
}

