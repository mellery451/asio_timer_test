
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/math/tools/univariate_statistics.hpp>
#include <boost/optional.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

int main(int, char**) {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    size_t const count = 100;
    boost::asio::io_service ios;
    boost::optional<boost::asio::io_service::work> work {ios};
    std::thread worker { [&]{ ios.run(); } };
    boost::asio::basic_waitable_timer<steady_clock> timer {ios};
    std::vector<steady_clock::duration::rep> elapsed_times;
    elapsed_times.reserve (count);
    std::mutex gate;

    std::cout << "========================================\n";
    std::cout << "Starting timer measurement...\n";
    for (auto samples = count; samples != 0 ; --samples)
    {
        auto const start {steady_clock::now()};
        timer.expires_after (100ms);
        gate.lock ();
        timer.async_wait ( [&] (boost::system::error_code const& ec) {
            if (ec)
                std::cerr <<
                    "!! got an ec in timer cb: " << ec.message() << std::endl;
            auto const end {steady_clock::now()};
            auto const elapsed {end - start};
            elapsed_times.emplace_back (
                duration_cast<milliseconds>(elapsed).count());
            gate.unlock ();
        });
        std::unique_lock <std::mutex> waithere {gate};

        if (samples % 10 == 0)
            std::cout << samples << "..."; std::cout.flush();
    }
    std::cout << "\n";
    work = boost::none;
    worker.join();

    std::cout << "Done.\n";
    std::cout << "========================================\n";
    std::cout << "Mean: " << boost::math::tools::mean(elapsed_times) << "ms\n";
    std::cout << "Median: " << boost::math::tools::median(elapsed_times) << "ms\n";
    std::cout << "Max: " << *(std::max_element(elapsed_times.begin(),elapsed_times.end())) << "ms\n";
    std::cout << "Min: " << *(std::min_element(elapsed_times.begin(),elapsed_times.end())) << "ms\n";
    std::cout << "========================================\n";

    return 0;
}
