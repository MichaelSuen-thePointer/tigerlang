#include <cassert>
#include "TigerFrame.hpp"
#include "TigerIRT.hpp"

namespace tiger
{
const std::string& Frame::currentLoopBreakLabel() const
{
    return _currentLoopBreakLabel;
}

Frame::Frame()
    : _frameBuffer(1)
    , _depth(0)
{
    _frameBuffer[0].reset(new TigerFrame(nullptr));
}

TigerFrame* Frame::currentFrame()
{
    return _frameBuffer[_depth].get();
}

void Frame::enterFrame()
{
    _depth++;
    if (_frameBuffer.size() == _depth)
    {
        _frameBuffer.emplace_back(new TigerFrame(_frameBuffer[_depth - 1].get()));
    }
    else
    {
        _frameBuffer[_depth].reset(new TigerFrame(_frameBuffer[_depth - 1].get()));
    }
}

void Frame::exitFrame()
{
    assert(_depth > 0);
    --_depth;
}

void Frame::addStringFragments(std::string label, std::string literal)
{
    assert(_stringFragments.find(label) == _stringFragments.end());
    _stringFragments[label] = literal;
}

void Frame::addFunction(std::string name, ir::IRTNode* body)
{
    assert(_functionFragments.find(name) == _functionFragments.end());
    _functionFragments[name].reset(body);
}

TigerFrame::TigerFrame(TigerFrame* staticLink)
    : _staticLink(staticLink)
    , _scopeOffset(1, 0)
    , _parameterOffset(2 * WordSize)
{
}

TigerFrame* TigerFrame::staticLink() const
{
    return _staticLink;
}

void TigerFrame::inScope()
{
    _scopeOffset.push_back(_scopeOffset.back());
}

void TigerFrame::outScope()
{
    _scopeOffset.pop_back();
}

void TigerFrame::addVariable(const std::string& name)
{
    _scopeOffset.back() -= WordSize;
    _variables[name] = _scopeOffset.back();
}

void TigerFrame::addParameter(const std::string& name)
{
    _parameterOffset += WordSize;
    _parameters[name] = _parameterOffset;
}

int TigerFrame::getOffset(const std::string& name)
{
    {
        auto r = _variables.find(name);
        if (r != _variables.end() && r->second >= _scopeOffset.back())
        { //ջ������offset�Ǹ���
            return r->second;
        }
    }
    {
        auto r = _parameters.find(name);
        if (r != _parameters.end())
        {
            return r->second;
        }
    }
    assert(0);
    return 0;
}

TigerFrame* TigerFrame::frameByName(const std::string& name)
{
    TigerFrame* current = this;
    for (; current != nullptr; current = current->staticLink())
    {
        {
            auto r = current->_variables.find(name);
            if (r != current->_variables.end() && r->second >= current->_scopeOffset.back())
            { //ջ������offset�Ǹ���
                return current;
            }
        }
        {
            auto r = current->_parameters.find(name);
            if (r != current->_parameters.end())
            {
                return current;
            }
        }
    }
    return nullptr;
}

FrameGuard::FrameGuard(Frame& f)
    : _f(f)
{
    _f.enterFrame();
}

FrameGuard::~FrameGuard()
{
    _f.exitFrame();
}

FrameScopeGuard::FrameScopeGuard(Frame& f)
    : _f(f)
#ifdef DEBUG
    , _currFrame(_f.currentFrame())
#endif
{
    _f.currentFrame()->inScope();
}

FrameScopeGuard::~FrameScopeGuard()
{
#ifdef DEBUG
    assert(_currFrame == _f.currentFrame());
#endif
    _f.currentFrame()->outScope();
}

void Frame::setLoopBreakLabel(std::string label)
{
    _currentLoopBreakLabel = std::move(label);
}

const std::map<std::string, std::unique_ptr<ir::IRTNode>>& Frame::functionFragments() const
{
    return _functionFragments;
}

int TigerFrame::currentOffset() const
{
    return _scopeOffset.back();
}

}
