#ifndef STARBRUSH_H
#define STARBRUSH_H
#include <brushes/brush.h>

class StarBrush : public Brush {
public:
    StarBrush(const std::string& name);
    virtual void SetHeight(unsigned int height);
    virtual void BrushBegin(const glm::vec2 pos) override;
    virtual void BrushMove(const glm::vec2 pos) override;
    virtual void BrushEnd(const glm::vec2 pos) override;

protected:
    QLabeledSlider* height_slider_;
    unsigned int GetHeight() const;
};
#endif // STARBRUSH_H
