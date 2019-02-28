//
// Created by rgerst on 28.02.19.
//

#pragma once


#include <string>
#include <memory>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace markdown::impl {

    /**
     * A markdown element
     */
    struct element {

        /**
         * Renders the markdown element as string
         * @return
         */
        virtual std::string to_string() const = 0;

    };

    /**
     * An element that wraps around another element
     * @tparam Element
     */
    template<typename Element, typename Base>
    struct wrapper : public Base {
        std::unique_ptr<Element> content;
        wrapper() = default;
        explicit wrapper(std::unique_ptr<Element> t_content) : content(std::move(t_content)) {

        }
    };

    /**
     * An inline-element
     */
    struct span : public element {
    };

    /**
     * An element that forms its own paragraph
     */
    struct paragraph : public element {
    };

    /**
     * An inline text
     */
    struct text : public span {
        std::string content;
        text() = default;

        explicit text(std::string t_content) : content(std::move(t_content)) {
        }

        std::string to_string() const override {
            return content;
        }
    };

    /**
     * An italic-formatted text
     */
    struct italic : public wrapper<text, span> {

        using wrapper<text, span>::wrapper;

        std::string to_string() const override {
            return "*" + content->to_string() + "*";
        }
    };

    /**
     * A bold-formatted text
     */
    struct bold : public wrapper<text, span> {

        using wrapper<text, span>::wrapper;

        std::string to_string() const override {
            return "**" + content->to_string() + "**";
        }
    };

    /**
     * Inline code
     */
    struct inline_code : public wrapper<text, span> {

        using wrapper<text, span>::wrapper;

        std::string to_string() const override {
            return "`" + content->to_string() + "`";
        }
    };

    /**
     * A link
     */
    struct link : public span {
        std::string content;
        std::string target;

        link() = default;

        explicit link(std::string t_content, std::string t_target) :
        content(std::move(t_content)), target(std::move(t_target)) {

        }

        std::string to_string() const override {
            return "[" + content + "](" + target + ")";
        }
    };

    /**
     * An image
     */
    struct image : public span {
        std::string alt;
        std::string target;

        image() = default;

        explicit image(std::string t_target, std::string t_alt = "") :
        alt(std::move(t_alt)), target(std::move(t_target)) {

        }

        std::string to_string() const override {
            return "![" + alt + "](" + target + ")";
        }
    };

    /**
     * An item list
     */
    struct itemize : public paragraph {
        std::vector<std::unique_ptr<span>> content;

        itemize() = default;
        explicit itemize(std::vector<std::unique_ptr<span>> t_content) : content(std::move(t_content)) {
        }

        std::string to_string() const override {
            std::string output = "";
            for(const auto &item : content) {
                output += "* " + item->to_string() + "\n";
            }
            return output;
        }
    };

    /**
     * An enumerated list
     */
    struct enumerate : public paragraph {
        std::vector<std::unique_ptr<span>> content;

        enumerate() = default;
        explicit enumerate(std::vector<std::unique_ptr<span>> t_content) : content(std::move(t_content)) {

        }

        std::string to_string() const override {
            std::string output = "";
            for(size_t i = 0; i < content.size(); ++i) {
                output += std::to_string(i + 1) + ". " + content.at(i)->to_string() + "";
            }
            return output;
        }
    };

    /**
     * A horizontal ruler
     */
    struct hruler : public paragraph {
        std::string to_string() const override {
            return "---";
        }
    };

    /**
     * A block of code
     */
    struct code : public wrapper<text, paragraph> {
        std::string language;
        code() = default;
        code(std::unique_ptr<text> t_content, std::string t_language) :
        wrapper<text, paragraph>(std::move(t_content)), language(std::move(t_language)) {

        }

        std::string to_string() const override {
            return "```" + language + "\n" + content->to_string() + "\n```";
        }
    };

    /**
     * A heading
     */
    struct heading : public wrapper<text, paragraph> {

        int depth = 0;

        heading() = default;
        explicit heading(std::unique_ptr<text> t_content, int t_depth = 0) :
        wrapper<text, paragraph>::wrapper(std::move(t_content)), depth(t_depth) {

        }

        std::string to_string() const override {
            if(depth == 0) {
                return content->to_string() + "\n" + std::string(content->to_string().length(), '=');
            }
            else if (depth == 1) {
                return content->to_string() + "\n" + std::string(content->to_string().length(), '-');
            }
            else {
                return std::string(depth + 1, '#') + " " + content->to_string();
            }
        }
    };

    /**
     * Paragraph that wraps around a span
     */
    struct span_paragraph : public paragraph {

        std::vector<std::unique_ptr<span>> content;

        template<typename Span>
        void operator += (Span entry) {
            content.emplace_back(std::move(entry));
        }

        template<typename Span>
        span_paragraph& operator +(Span entry) {
            *this += std::move(entry);
            return *this;
        }

        template <typename Span, typename ...Args>
        void addAll(Span &&entry, Args&&... args) {
            *this += std::forward<Span>(entry);
            if constexpr (sizeof...(Args) > 0) {
                addAll(std::forward<Args>(args)...);
            }
        }

        std::string to_string() const override {
            std::string output;
            for(const auto &s : content) {
                output += s->to_string();
            }

            constexpr int line_width = 80;

            if(output.length() <= line_width)
                return output;

            // Apply word wrapping
            std::vector<std::string> words;
            boost::split(words, output, boost::is_any_of(" "));
            output = "";
            std::string current_line;

            for(const auto &word : words) {
                if(word.length() > line_width) {
                    output += current_line + "\n";
                    output += word;
                    current_line = "";
                }
                else if(current_line.length() + word.length() > line_width) {
                    output += current_line + "\n";
                    current_line = word + " ";
                }
                else {
                    current_line += word + " ";
                }
            }

            output += current_line;
            boost::trim_if(output, boost::is_any_of(" \n"));

            return output;
        }
    };

    /**
     *  A markdown document builder
     */
    struct document : public element {

        std::vector<std::unique_ptr<paragraph>> content;

        template<typename Paragraph>
        void operator += (std::unique_ptr<Paragraph> entry) {
            static_assert(std::is_base_of<paragraph, Paragraph>::value, "You can only add paragraphs");
            content.emplace_back(std::move(entry));
        }

        template<typename Paragraph>
        document& operator +(std::unique_ptr<Paragraph> entry) {
            *this += std::move(entry);
            return *this;
        }

        std::string to_string() const override {
            std::string output;
            for(const auto &paragraph : content) {
                output += paragraph->to_string() + "\n\n";
            }
            return output;
        }

    };
}

namespace markdown {
    using document = markdown::impl::document;

    template <typename Span, typename ...Args>
    inline std::unique_ptr<markdown::impl::span_paragraph> paragraph(Span &&span, Args&&... args) {
        auto result = std::make_unique<markdown::impl::span_paragraph>();
        result->addAll(std::forward<Span>(span), std::forward<Args>(args)...);
        return result;
    }

    inline std::unique_ptr<markdown::impl::span_paragraph> paragraph(std::unique_ptr<markdown::impl::span> span) {
        auto result = std::make_unique<markdown::impl::span_paragraph>();
        *result += std::move(span);
        return result;
    }

    inline std::unique_ptr<markdown::impl::text> text(std::string content) {
        return std::make_unique<markdown::impl::text>(std::move(content));
    }

    inline std::unique_ptr<markdown::impl::heading> heading(std::string title, int level = 0) {
        return std::make_unique<markdown::impl::heading>(text(std::move(title)), level);
    }

    inline std::unique_ptr<markdown::impl::heading> heading1(std::string title) {
        return heading(std::move(title), 0);
    }

    inline std::unique_ptr<markdown::impl::heading> heading2(std::string title) {
        return heading(std::move(title), 1);
    }

    inline std::unique_ptr<markdown::impl::italic> italic(std::string content) {
        return std::make_unique<markdown::impl::italic>(text(std::move(content)));
    }

    inline std::unique_ptr<markdown::impl::bold> bold(std::string content) {
        return std::make_unique<markdown::impl::bold>(text(std::move(content)));
    }

    inline std::unique_ptr<markdown::impl::code> code(std::string content, std::string language = "") {
        return std::make_unique<markdown::impl::code>(text(std::move(content)), std::move(language));
    }
}


