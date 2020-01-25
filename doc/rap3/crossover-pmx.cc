inline static void
pmx_patch_(Path &p, const Path &p1, const Path &p2, size_t i, size_t xbegin, size_t xend)
FORCE_INLINE
{
	auto oldnode = p[i];
	point_type newnode;

	const point_type *newnode_p = &p[i];
	while(1)
	{
		newnode = *newnode_p;
		newnode_p = std::find(&p2[xbegin], &p2[xend], oldnode);

		if (newnode_p == &p2[xend])
			break;

		oldnode = p1[newnode_p - &p2[0]];
	}

	p[i] = oldnode;
}

Path
cross_pmx(const Path &p1, const Path &p2)
{
	const auto path_size = std::size(p1);
	Path p(p1);

	// If range starts at the end, then its bad range, so avoid
	// that with shrinking span of path_size.
	auto xbegin = std::rand() % (path_size - 1);

	// xend should point to past last element
	auto xend   = std::rand() % (path_size + 1);

	if (UNLIKELY(xbegin == xend)) ++xend;
	else if (xbegin > xend) std::swap(xbegin, xend);

	std::fill(&p[xbegin], &p[xend], 1337);

	for (auto i = 0ull; i < xbegin; ++i) {
		pmx_patch_(p, p1, p2, i, xbegin, xend);
	}

	for (auto i = xend; i < path_size; ++i) {
		pmx_patch_(p, p1, p2, i, xbegin, xend);
	}

	std::copy(&p2[xbegin], &p2[xend], &p[xbegin]);

	return p;
}
