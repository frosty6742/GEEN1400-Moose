// LineFollowerPID.hpp
#pragma once

class LineFollowerPID {
public:
  LineFollowerPID(float kp, float ki, float kd);

  // Call this every update cycle
  float update(bool left_on_line, bool right_on_line, float dt);

private:
  float kp_, ki_, kd_;
  float last_error_;
  float integral_;

  float calculateError(bool left_on_line, bool right_on_line);
};
