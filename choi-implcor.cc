void ChoiceRunnerImpl::process_input()
{
    std::string input;
    std::getline(is_, input);

    const auto* current = story_->get_current();
    const int n = static_cast<int>(current->list_choices(true).size());

    int choice_idx = 0;
    try
    {
        std::size_t pos = 0;
        choice_idx = std::stoi(input, &pos);

        // Vérifie que toute la chaîne a été consommée
        // "1 toto" -> pos=1, input.size()=6 -> invalide
        if (pos != input.size())
            throw RunnerInterrupt("Please input an integer between 1 and "
                                  + std::to_string(n));
    }
    catch (const RunnerInterrupt&)
    {
        throw; // Re-propage notre propre exception
    }
    catch (const std::exception&)
    {
        throw RunnerInterrupt("Please input an integer between 1 and "
                              + std::to_string(n));
    }

    if (choice_idx < 1 || choice_idx > n)
        throw RunnerInterrupt("Please input an integer between 1 and "
                              + std::to_string(n));

    const auto& node_impl = story::to_impl(*current);
    const std::size_t total_raw = current->list_choices(false).size();
    int visible_count = 0;
    for (std::size_t i = 0; i < total_raw; ++i)
    {
        if (!node_impl.check_conditions(i))
            continue;

        ++visible_count;
        if (visible_count == choice_idx)
        {
            const auto* next = current->get_choice(i, true);
            if (next)
                story_->set_current(next);
            return;
        }
    }

    throw RunnerInterrupt("Please input an integer between 1 and "
                          + std::to_string(n));
}
