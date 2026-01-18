#include "Direction.h"
#include "Float3.h"
#include "Model.h"
#include "Shader.h"
#include "Unit.h"
#include <memory>

Unit::Unit(const Direction& direction, const Float3& position, const Float3& offset_camera, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader)
	: _direction(direction), _position(position), _offset_camera(offset_camera),
	_model(model), _shader(shader) {
}
