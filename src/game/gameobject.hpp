namespace game {

class GameObject {
  public:
    void set_pos(int x_pos, int y_pos);
    // This is JUST for updating the position because of velocity.
    void do_pos_updates();

  protected:
    float x, y;
    float vx, vy;
};

} // namespace game
