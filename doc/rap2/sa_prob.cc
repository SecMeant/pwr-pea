double
calc_prob(double temperature,
          double newcost,
          double oldcost)
{
  return std::exp(-(newcost - oldcost) / temperature);
}
