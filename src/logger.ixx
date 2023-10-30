module;

#include <filesystem>
// #include <stacktrace>
#include <string_view>
#include <utility>

#include <defines.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

export module logger;

export namespace logger
{
    using level = spdlog::level::level_enum;

    class Logger
    {
        template<level lvl, typename... Args>
        friend void log(std::string_view const& msg, Args&&... args);

    public:
        static auto get() -> std::shared_ptr<spdlog::logger> const& { return m_logger; }

    private:
        static void log(std::string_view msg,
                        spdlog::level::level_enum level,
                        char const* file,
                        int line)
        {
            constexpr size_t kRootDirPathLength =
                std::string_view(ROOT_SOURCE_DIR).length() + 1;

            m_logger->log(
                spdlog::source_loc { file + kRootDirPathLength, line, "hi" },
                level,
                msg
            );
        }

        static auto create() -> std::shared_ptr<spdlog::logger>;

        inline static std::shared_ptr<spdlog::logger> const m_logger { create() };
    };

    template<level lvl, typename... Args>
    ALWAYS_INLINE void log(std::string_view const& msg, Args&&... args)
    {
        if constexpr (lvl >= SPDLOG_ACTIVE_LEVEL)
        {
            // auto const stacktrace = std::stacktrace::current()[0];
            Logger::log(
                fmt::vformat(msg, fmt::make_format_args(std::forward<Args>(args)...)),
                lvl,
                "",
                0);
        }
    }

    template<typename... Args>
    ALWAYS_INLINE void trace(std::string_view const& msg, Args&&... args)
    {
        log<level::trace>(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    ALWAYS_INLINE void debug(std::string_view const& msg, Args&&... args)
    {
        log<level::debug>(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    ALWAYS_INLINE void info(std::string_view const& msg, Args&&... args)
    {
        log<level::info>(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    ALWAYS_INLINE void warn(std::string_view const& msg, Args&&... args)
    {
        log<level::warn>(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    ALWAYS_INLINE void error(std::string_view const& msg, Args&&... args)
    {
        log<level::err>(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    ALWAYS_INLINE void critical(std::string_view const& msg, Args&&... args)
    {
        log<level::critical>(msg, std::forward<Args>(args)...);
    }

    auto Logger::create() -> std::shared_ptr<spdlog::logger>
    {
        using namespace std::chrono_literals;

        auto logFilePath { std::filesystem::current_path() / "playground.log" };

        spdlog::level::level_enum level { SPDLOG_ACTIVE_LEVEL };

        std::vector<spdlog::sink_ptr> sinks {
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(),
                                                                true)
        };

        for (auto& sink : sinks)
        {
#ifdef NDEBUG
            sink->set_pattern("%^[%l] [%r]: %v%$");
#else
            sink->set_pattern("%^[%l] [%r] %@: %v%$");
#endif
        }

        auto logger =
            std::make_shared<spdlog::logger>("MAIN", sinks.begin(), sinks.end());

        spdlog::register_logger(m_logger);

        logger->set_level(level);

        spdlog::flush_every(2s);

        return logger;
    }
}  // namespace logger