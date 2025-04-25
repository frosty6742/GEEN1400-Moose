#ifndef LINETRACKER_H
#define LINETRACKER_H

enum class LinePosition {
    CENTERED,
    UNDER_LEFT,
    UNDER_RIGHT,
    OUTSIDE_LEFT,
    OUTSIDE_RIGHT,
    LOST
};

class LineTracker {
public:
    LineTracker();

    // Returns the current position of the line based on sensor readings
    LinePosition detectPosition(bool leftOnLine, bool rightOnLine);

    // Optional: Get last known position
    LinePosition getLastKnownPosition() const;

private:
    LinePosition lastPosition;
};

#endif // LINETRACKER_H

