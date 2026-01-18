#pragma once
#include "Direction.h"
#include "Float3.h"
#include "Model.h"
#include "Shader.h"
#include <memory>

class Unit {
public:
	Unit(const Direction& direction, const Float3& position, const Float3& offset_camera, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);
	const Direction& Get_Direction() const { return _direction; }
	const Float3& Get_Position() const { return _position; }
	const Float3& Get_Offset_Camera() const { return _offset_camera; }
	Direction& Modify_Direction() { return _direction; }
	Float3& Modify_Position() { return _position; }
	Model& Get_Model() const { return *_model; }
	Shader& Get_Shader() const { return *_shader; }
private:
	Direction _direction;
	Float3 _position;
	Float3 _offset_camera;
	std::shared_ptr<Model> _model;
	std::shared_ptr<Shader> _shader;
};

