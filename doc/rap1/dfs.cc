namespace pea {
  void
  tspdfs::TSP(size_t v) noexcept
  {
    size_t u;
    lpath.push_back(v);

    if (lpath.size() < matrix.size()) {

      visited[v] = true;

      for (u = 0; u < matrix.size(); u++) {

        if (!visited[u]) {
          lcost += matrix.get(v, u);
          if (lcost < rcost)
            TSP(u);
          lcost -= matrix.get(v, u);
        }
      }

      visited[v] = false;

    } else { 
      // Path is complete, close it
      lcost += matrix.get(v, 0);

      // If local result is better than currently best
      // Copy local to best
      if (lcost < rcost) {
        rcost = lcost;
        rpath = lpath;
      }

      lcost -= matrix.get(v, 0);
    }

    lpath.pop_back();
  }
} // namespace pea
