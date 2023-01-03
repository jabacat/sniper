namespace game {

class GameObject {
  public:
    void set_pos(int x_pos, int y_pos);

  protected:
    float x, y;
    float vx, vy;
};

} // namespace game
