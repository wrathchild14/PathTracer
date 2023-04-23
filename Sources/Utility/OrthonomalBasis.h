#pragma once

class ONB
{
public:
	ONB()
	{
	}

	Vec3 operator[](int i) const { return axis_[i]; }

	Vec3 u() const { return axis_[0]; }
	Vec3 v() const { return axis_[1]; }
	Vec3 w() const { return axis_[2]; }

	Vec3 local(double a, double b, double c) const
	{
		return a * u() + b * v() + c * w();
	}

	Vec3 local(const Vec3& a) const
	{
		return a.x() * u() + a.y() * v() + a.z() * w();
	}

	void BuildFromW(const Vec3&);

private:
	Vec3 axis_[3];
};


inline void ONB::BuildFromW(const Vec3& n)
{
	axis_[2] = UnitVector(n);
	const Vec3 a = (fabs(w().x()) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
	axis_[1] = UnitVector(Cross(w(), a));
	axis_[0] = Cross(w(), v());
}
