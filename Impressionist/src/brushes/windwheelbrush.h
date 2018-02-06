#ifndef WINDWHEELBRUSH_H
#define WINDWHEELBRUSH_H
#include <brushes/brush.h>
class WindWheelBrush : public Brush {
public:
    WindWheelBrush(const std::string& name);
    virtual void SetWidth(unsigned int width);
    virtual void BrushBegin(const glm::vec2 pos) override;
    virtual void BrushMove(const glm::vec2 pos) override;
    virtual void BrushEnd(const glm::vec2 pos) override;

protected:
    QLabeledSlider* width_slider_;
    unsigned int GetWidth() const;
};
#endif // WINDWHEELBRUSH_H
