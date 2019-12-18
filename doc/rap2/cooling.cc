void
regulate_temperature(float &temp, size_t time)
{
  if constexpr (tproc == TempProcType::standard) {

    temp = initial_temperature - (cooling_rate * time);

  } else if constexpr (tproc == TempProcType::log) {

    auto temp_copy = temp;
    temp = initial_temperature / std::log(time + 1);
    if (temp == temp_copy)
      temp = end_temperature;

  } else if constexpr (tproc == TempProcType::geometric) {

    temp = initial_temperature * std::pow(cooling_rate, time);
  }
}
