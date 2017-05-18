#pragma once
#include "TigerAST.hpp"

namespace tiger
{

class TigerFrame;
class Frame
{
private:
    std::vector<std::unique_ptr<TigerFrame>> _frameBuffer;
    size_t _depth;
    std::map<std::string, std::string> _stringFragments;
public:
    Frame();

    TigerFrame* currentFrame();

    void enterFrame();

    void exitFrame();

    void addStringFragments(std::string label, std::string literal);
};

class TigerFrame
{
protected:
    TigerFrame* _staticLink;
    std::map<std::string, int> _parameters;
    std::map<std::string, int> _variables;
    std::vector<int> _scopeOffset;
    int _parameterOffset;

public:
    constexpr static int WordSize = 4;
    constexpr static int StaticLinkOffset = 8;
    explicit TigerFrame(TigerFrame* staticLink);

    TigerFrame* staticLink() const;

    void inScope();

    void outScope();

    void addVariable(const std::string& name);

    void addParameter(const std::string& name);

    int getOffset(const std::string& name);

    TigerFrame* frameByName(const std::string& name);
};

class FrameGuard
{
    Frame& _f;
public:
    explicit FrameGuard(Frame& f);

    ~FrameGuard();
};

class FrameScopeGuard
{
    Frame& _f;
#ifdef DEBUG
    TigerFrame* _currFrame;
#endif
public:
    explicit FrameScopeGuard(Frame& f);

    ~FrameScopeGuard();
};

}