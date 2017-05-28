#pragma once
#include <vector>
#include <memory>
#include <map>
#include <iostream>

namespace tiger
{

class TigerFrame;
namespace ir
{
class IRTNode;
}
class Frame final
{
private:
    std::vector<std::unique_ptr<TigerFrame>> _frameBuffer;
    size_t _depth;
    std::map<std::string, std::string> _stringFragments;
    std::map<std::string, std::unique_ptr<ir::IRTNode>> _functionFragments;
    std::string _currentLoopBreakLabel;

public:
    Frame();
    
    ~Frame();

    TigerFrame* currentFrame();

    void enterFrame();

    void exitFrame();

    void addStringFragments(std::string label, std::string literal);

    void addFunction(std::string name, ir::IRTNode* body);

    const std::string& currentLoopBreakLabel() const;
    
    void setLoopBreakLabel(std::string label);

    const std::map<std::string, std::unique_ptr<ir::IRTNode>>& functionFragments() const;

    const std::map<std::string, std::string>& stringFragments() const;

    void generatePlan9(std::string text, std::ostream& out);
};

/*
+-----------+
|param 2    |    +32
+-----------+
|param 1    |    +24  
+-----------+
|static link|    +16
+-----------+
|return addr|    +8
+-----------+
|caller rbp | <- rbp
+-----------+
|local var 1|    -8
+-----------+
|local var 1|    -16 <- rsp
+-----------+
*/

class TigerFrame
{
protected:
    TigerFrame* _staticLink;
    std::map<std::string, int> _parameters;
    std::map<std::string, int> _variables;
    std::vector<int> _scopeOffset;
    int _parameterOffset;

public:
    constexpr static int WordSize = 8;
    constexpr static int StaticLinkOffset = 2 * WordSize;
    explicit TigerFrame(TigerFrame* staticLink);

    TigerFrame* staticLink() const;

    void inScope();

    void outScope();

    void addVariable(const std::string& name);

    void addParameter(const std::string& name);

    int getOffset(const std::string& name);

    int currentOffset() const;

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
