#include "tool_place_refdes_and_value.hpp"
#include "imp/imp_interface.hpp"
#include <iostream>
#include "nlohmann/json.hpp"

namespace horizon {

ToolPlaceRefdesAndValue::ToolPlaceRefdesAndValue(Core *c, ToolID tid) : ToolBase(c, tid)
{
}

bool ToolPlaceRefdesAndValue::can_begin()
{
    return core.y;
}

void ToolPlaceRefdesAndValue::update_texts(const Coordi &c)
{
    text_refdes->placement.shift = {c.x, std::abs(c.y)};
    text_value->placement.shift = {c.x, -std::abs(c.y)};
}

ToolResponse ToolPlaceRefdesAndValue::begin(const ToolArgs &args)
{

    text_refdes = core.r->insert_text(UUID::random());
    text_refdes->layer = 0;
    text_refdes->text = "$REFDES";

    text_value = core.r->insert_text(UUID::random());
    text_value->layer = 0;
    text_value->text = "$VALUE";
    imp->tool_bar_set_tip("<b>LMB:</b>place text <b>RMB:</b>cancel ");

    update_texts(args.coords);

    return ToolResponse();
}
ToolResponse ToolPlaceRefdesAndValue::update(const ToolArgs &args)
{
    if (args.type == ToolEventType::MOVE) {
        update_texts(args.coords);
    }
    else if (args.type == ToolEventType::CLICK) {
        if (args.button == 1) {
            core.r->selection.clear();
            core.r->selection.emplace(text_value->uuid, ObjectType::TEXT);
            core.r->selection.emplace(text_refdes->uuid, ObjectType::TEXT);
            core.r->commit();
            return ToolResponse::end();
        }
        else if (args.button == 3) {
            core.r->revert();
            return ToolResponse::end();
        }
    }
    return ToolResponse();
}
} // namespace horizon
