#include "video/render/forward_rendering.h"

#include "engine.h"

namespace video::render {
ForwardRendering::ForwardRendering() = default;

ForwardRendering::~ForwardRendering() = default;

void ForwardRendering::Render(
    State &state, const std::vector<Renderable *> &renderables) const {
  auto transparent = std::vector<Renderable *>();
  transparent.reserve(renderables.size());
  for (const auto &renderable : renderables) {
    if (renderable->IsOpaque()) {
      state.SetBlend(false);
      renderable->Render();
    } else {
      transparent.emplace_back(renderable);
    }
  }
  for (const auto &renderable : transparent) {
    state.SetBlend(true);
    renderable->Render();
  }
  /*for (const auto &renderable: renderables) {
      // auto pile = piles.emplace_back(pair.first, pair.second);
      //pile.GetRenderObject()->SetInstancesPrepared(0); // Reset
      //pile.New();
      //pile.Flush();
      //  pair.first->SetInstancesPrepared(0);
      // object->Prepare();

      if (state.GetBlend() == renderable->IsOpaque()) {
          //glEnable(GL_BLEND);
          //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          state.SetBlend(!renderable->IsOpaque());
      }
      renderable->Render();
  }*/
  // glDisable(GL_BLEND);
  return;
  // STOP_MEASURE_TIME("ForwardRendering New");
  //    m_Prepared = true;
  //   }
  /*for (RenderPile& renderPile : renderPileVector) {
    renderPile.New();
  }*/
  // glDisable(GL_BLEND);

  /*for (std::vector<Pile> piles; Pile &pile: piles) {
      if (!state.GetBlend() && !pile.IsOpaque()) {
          /*glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          blendEnabled = true;*/
  state.SetBlend(true);
  //}
  /*pile.Render();0
  // firstInstance->GetRenderObject()->GetRenderer()->Render(renderList);
}*/
}

/*void ForwardRendering::toSortedList(std::vector<object::Instance *>
   &instances, std::vector<Pile> &piles) { std::vector<Pile>
   transparentRenderPileList; std::vector<Pile> opaqueRenderPileList;
    std::vector<object::Instance *> transparentInstanceList;

    std::ranges::sort(instances, SortByOpaqueAndRenderObject);
    const object::Object *prevRenderObject = nullptr;
    Pile *currentPile = nullptr;
    for (object::Instance *instance: instances) {
        if (object::Object *renderObject = instance->GetRenderObject();
   prevRenderObject != renderObject) { prevRenderObject = renderObject;
            currentPile = &piles.emplace_back(renderObject);
        }
        currentPile->Add(instance);
    }

    for (Pile &renderPile: piles) {
        //flatList.insert(flatList.end(), list.begin(), list.end());
        //std::vector<Renderobject::Instance*> opaqueInstanceList;
        //std::vector<Renderobject::Instance*> transparentInstanceList;
        /*  for (Renderobject::Instance* scene : renderPile.getInstances()) {
            if (scene->IsOpaque()) {
              opaqueInstanceList.emplace_back(scene);
            } else {
              transparentInstanceList.emplace_back(scene);
            }
          }*/
/*  if (!opaqueInstanceList.empty()) {
    opaqueRenderPileList.emplace_back(renderPile.getRenderObject(),
  opaqueInstanceList);
  }
  if (!transparentInstanceList.empty()) {
    transparentRenderPileList.emplace_back(renderPile.GetRenderObject(),
  transparentInstanceList);
  }
  if (!opaqueInstanceList.empty()
          && !transparentInstanceList.Empty()) {
    opaqueRenderPileList.back().setBufferOffset(0);
    opaqueRenderPileList.back().setLastPile(false);
    transparentRenderPileList.back().setBufferOffset(opaqueInstanceList.size());
    transparentRenderPileList.back().setLastPile(true);
  }*/
//    renderPile.GetRenderObject()->SetInstancesPrepared(0); // Reset
/* if (renderPile.IsOpaque()) {
     opaqueRenderPileList.emplace_back(renderPile);
     renderPile.Prepare();
     renderPile.Flush();
 } else {
     transparentInstanceList.insert(transparentInstanceList.end(),
                                    renderPile.GetInstances().begin(),
                                    renderPile.GetInstances().end());
     // transparentRenderPileList.emplace_back(renderPile);
     //renderPile.New();
     //renderPile.flush();
 }
}
// renderPileVector = std::move(opaqueRenderPileList);
//renderPileVector.insert(renderPileVector.end(),
transparentRenderPileList.begin(), transparentRenderPileList.end());
//return;
// auto pipeline =
engine::Engine::GetInstance().GetVideoManager()->GetActivePipeline();
// if (pipeline) {
//   auto viewer = pipeline->getViewer();
// viewer->SortFarToNear(transparentInstanceList);
//}
//std::sort(transparentInstanceList.begin(), transparentInstanceList.end(),
SortByDistanceToCameraComparaor::compare);

//std::vector<Renderobject::Instance*> tmpList;
/*const object::Object *prevObject = nullptr;
for (auto *instance: transparentInstanceList) {
 if (auto *currentObject = instance->GetRenderObject(); currentObject !=
prevObject) { prevObject = currentObject; if (currentPile != nullptr) {
         currentPile->Prepare();
     }
     //  if (!tmpList.Empty()) {
     //std::vector<Renderobject::Instance*> instanceList = std::move(tmpList);
     currentPile = &transparentRenderPileList.emplace_back(currentObject);
     //transparentRenderPileList.back().New();
     //}
 }
 currentPile->Add(instance);
 //tmpList.push_back(instance);
}
if (!transparentRenderPileList.empty()) {
 transparentRenderPileList.back().Prepare();
}
for (Pile &renderPile: transparentRenderPileList) {
 renderPile.Flush();
}
/*  if (!tmpList.Empty()) {
 transparentRenderPileList.emplace_back(currentObject, tmpList);
 transparentRenderPileList.back().New();
}*/
/*
        piles = std::move(opaqueRenderPileList);
        piles.insert(piles.end(), transparentRenderPileList.begin(),
   transparentRenderPileList.end());
        //renderObjects.Clear();
        //renderObjects.insert(renderObjects.end(), opaqueList.begin(),
   opaqueList.end());
        //renderObjects.insert(renderObjects.end(), transparentList.begin(),
   transparentList.end()); return;
        /*std::sort(flatList.begin(), flatList.end(),
   SortByDistanceToCameraComparaor::compare);
        std::vector<Renderobject::Instance*> tmpList;
        RenderObject_Ptr currentObject = nullptr;
        for (Renderobject::Instance* instance : flatList) {
          if (instance->getRenderObject() == currentObject) {
            tmpList.push_back(instance);
          } else {
            if (!tmpList.empty()) {
              renderObjects.push_back(tmpList);
            }
            tmpList.Clear();
            tmpList.push_back(instance);
            currentObject = instance->GetRenderObject();
          }
        }
        if (!tmpList.Empty()) {
          renderObjects.push_back(tmpList);
        }*/
/*}/*

bool ForwardRendering::SortByOpaqueAndRenderObject(object::Instance *a,
object::Instance *b) { if (a->GetRenderObject()->IsOpaque() ==
b->GetRenderObject()->IsOpaque()) { return a->GetRenderObject() >
b->GetRenderObject();
    }
    return a->GetRenderObject()->IsOpaque();
}*/
} // namespace video::render
