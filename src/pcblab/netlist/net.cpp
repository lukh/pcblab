#include "net.h"

NetEntry::eEntryType NetEntry::getType() const
{
    return mType;
}

void NetEntry::setType(const eEntryType &inType)
{
    mType = inType;
}

string NetEntry::getDesignator() const
{
    return mDesignator;
}

void NetEntry::setDesignator(const string &inDes)
{
    mDesignator = inDes;
}

uint32_t NetEntry::getPin() const
{
    return mPin;
}

void NetEntry::setPin(uint32_t inPin)
{
    mPin = inPin;
}

double NetEntry::getHoleSize() const
{
    return mHoleSize;
}

void NetEntry::setHoleSize(double inHoleSize)
{
    mHoleSize = inHoleSize;
}

plPoint NetEntry::getPosition() const
{
    return mPosition;
}

void NetEntry::setPosition(const plPoint &inPosition)
{
    mPosition = inPosition;
}

uint32_t NetEntry::getAccessSide() const
{
    return mAccessSide;
}

void NetEntry::setAccessSide(uint32_t accessSide)
{
    mAccessSide = accessSide;
}

bool NetEntry::getPlated() const
{
    return mPlated;
}

void NetEntry::setPlated(bool plated)
{
    mPlated = plated;
}

double NetEntry::getFeatW() const
{
    return mFeatW;
}

void NetEntry::setFeatW(double inFW)
{
    mFeatW =inFW;
}

double NetEntry::getFeatH() const
{
    return mFeatH;
}

void NetEntry::setFeatH(double inFW)
{
    mFeatH = inFW;
}

double NetEntry::getFeatRot() const
{
    return mFeatRot;
}

void NetEntry::setFeatRot(double featRot)
{
    mFeatRot = featRot;
}

bool NetEntry::getIsDrilled() const
{
    return mIsDrilled;
}

void NetEntry::setIsDrilled(bool isDrilled)
{
    mIsDrilled = isDrilled;
}

bool NetEntry::getMidPoint() const
{
    return mMidPoint;
}

void NetEntry::setMidPoint(bool midPoint)
{
    mMidPoint = midPoint;
}
