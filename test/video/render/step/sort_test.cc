#include "video/render/step/sort.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <oneapi/tbb/detail/_task.h>

namespace soil::video::render::step {
    class RenderableObjectMock : public RenderableObject {
    public:
        explicit RenderableObjectMock(const float distance = 0.F, const bool sortable = true) :
            Distance(distance), Sortable(sortable) {}

        void Render(State& state) override {
            RenderCalledCount++;
        }

        float DistanceTo(const glm::vec3& point) override {
            return Distance;
        }

        bool IsSortable() const override {
            return Sortable;
        }

        int RenderCalledCount = 0;
        float Distance = 0.F;
        bool Sortable;
    };

    class SortTest : public testing::Test {};

    TEST_F(SortTest, Constructor) {
        auto sort = Sort("sort");

        EXPECT_EQ(sort.GetId(), "sort");
        EXPECT_EQ(sort.GetType(), Type::Prepare);
        EXPECT_EQ(sort.GetOrder(), Sort::Order::FrontToBack);
    }

    TEST_F(SortTest, ProcessEmptyRenderables) {
        auto sort = Sort("sort", Sort::Order::BackToFront);
        std::vector<RenderableObject*> renderables = {};
        auto ctx = Context {.Renderables = &renderables};

        sort.Process(ctx);

        EXPECT_TRUE(ctx.Renderables->empty());
    }

    TEST_F(SortTest, ProcessFrontToBack) {
        auto* near = new RenderableObjectMock(1.F);
        auto* middle = new RenderableObjectMock(3.F);
        auto* not_sortable = new RenderableObjectMock(99.F, false);
        auto* far = new RenderableObjectMock(6.F);
        std::vector<RenderableObject*> renderables = {near, far, not_sortable, middle};
        auto ctx = Context {
            .Renderables = &renderables,
        };
        auto sort = Sort("sort", Sort::Order::FrontToBack);

        sort.Process(ctx);

        EXPECT_THAT(*ctx.Renderables, testing::ElementsAre(not_sortable, near, middle, far));
    }

    TEST_F(SortTest, ProcessBackToFront) {
        auto* near = new RenderableObjectMock(1.F);
        auto* middle = new RenderableObjectMock(3.F);
        auto* not_sortable = new RenderableObjectMock(99.F, false);
        auto* far = new RenderableObjectMock(6.F);
        std::vector<RenderableObject*> renderables = {near, far, not_sortable, middle};
        auto ctx = Context {
            .Renderables = &renderables,
        };
        auto sort = Sort("sort", Sort::Order::BackToFront);

        sort.Process(ctx);

        EXPECT_THAT(*ctx.Renderables, testing::ElementsAre(not_sortable, far, middle, near));
    }
} // namespace soil::video::render::step
