ScoredPath
cross_ox(const ScoredPath &p1, const ScoredPath &p2)
{
	const auto path_size = std::size(p1);
	ScoredPath p(path_size);

	// If range starts at the end, then its bad range, so avoid
	// that with shrinking span of path_size.
	auto xbegin = std::rand() % (path_size - 1);

	// xend should point to past last element
	auto xend   = std::rand() % (path_size + 1);

	if (UNLIKELY(xbegin == xend)) ++xend;
	if (xbegin > xend) std::swap(xbegin, xend);

	// Fill with invalid nodes
	std::fill_n(std::begin(p), p.size(), 999999);

	// Copy random part from p1 to child
	std::copy(&p1[xbegin], &p1[xend], &p[xbegin]);

	// Copy each node from p2 that is not in child
	auto j = xend;
	for (auto &e : p2) {
		if (std::find(std::begin(p), std::end(p), e) == std::end(p)) {
			if (j == path_size)
				j = 0;

			p[j] = e;
			++j;
		}
	}

	return p;
}
