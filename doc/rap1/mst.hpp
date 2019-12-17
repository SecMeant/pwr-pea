namespace pea {
  class MSTMatrix
  {
  private:
    int64_t *m_data;
    size_t m_size;

  public:
    MSTMatrix() noexcept
      : MSTMatrix(0)
    {}
    MSTMatrix(int32_t size) noexcept;
    MSTMatrix(MSTMatrix &&mm) noexcept;
    ~MSTMatrix();

    void
    add_single(Edge edge) noexcept;

    void
    add(Edge edge) noexcept;

    int32_t
    get(size_t x, size_t y) const noexcept;

    void
    set(size_t x, size_t y, int64_t val);

    // Clears and resizes matrix
    void
    resize(size_t newsize) noexcept;

    void
    display() const noexcept;

    // Returns matrix with zero size on failure
    static MSTMatrix
    build_from_file(const char *filename);

    constexpr auto
    size() const noexcept
    {
      return this->m_size;
    }

    constexpr auto
    data() const noexcept
    {
      return this->m_data;
    }
  };

}; // namespace pea
