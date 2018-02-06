#ifndef TRIPLEHEXBRUSH_H
#define TRIPLEHEXBRUSH_H
#include <brushes/brush.h>
class TripleHexBrush : public Brush {
public:
    TripleHexBrush(const std::string& name);
    virtual void BrushBegin(const glm::vec2 pos) override;
    virtual void BrushMove(const glm::vec2 pos) override;
    virtual void BrushEnd(const glm::vec2 pos) override;
};
#endif // TRIPLEHEXBRUSH_H
