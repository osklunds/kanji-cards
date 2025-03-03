
// Copyright (C) 2025 Oskar Lundström

// This file is part of kanji-cards.

// kanji-cards is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.

// kanji-cards is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.

// You should have received a copy of the GNU General Public License along with
// kanji-cards. If not, see <https://www.gnu.org/licenses/>.

#include "create_card.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include "frequency.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <optional>
#include <utility>

const std::string out_dir = "out";
const int num_threads = 8;

TEST_CASE("create_card") {
    pugi::xml_document kanjidic2_doc {};
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("kanjidic2.xml");
    REQUIRE(result_kanjidic2 == true);

    pugi::xml_document jmdict_e_doc {};
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("JMdict_e.xml");
    REQUIRE(result_jmdict_e == true);

    // 生 has lots of words and words wrapping lines
    kanji_data kanji_data1 { "生", 1, kanjidic2_doc, jmdict_e_doc };

    // 働 has multiple lines of stroker order graphs
    kanji_data kanji_data2 { "働", 1234, kanjidic2_doc, jmdict_e_doc };

    std::filesystem::remove_all(out_dir);
    REQUIRE(!std::filesystem::exists(out_dir));
    REQUIRE(std::filesystem::create_directory(out_dir));

    std::string exp_kanji1_path = out_dir + "/0001-生.pdf";
    std::string exp_kanji2_path = out_dir + "/1234-働.pdf";

    REQUIRE(std::filesystem::exists(out_dir));
    REQUIRE(!std::filesystem::exists(exp_kanji1_path));
    REQUIRE(!std::filesystem::exists(exp_kanji2_path));

    REQUIRE(exp_kanji1_path == create_card(kanji_data1, out_dir));
    REQUIRE(exp_kanji2_path == create_card(kanji_data2, out_dir));

    REQUIRE(std::filesystem::exists(out_dir));
    REQUIRE(std::filesystem::exists(exp_kanji1_path));
    REQUIRE(std::filesystem::exists(exp_kanji2_path));
}

// A convenient way to run the main function
TEST_CASE("run_main", "[.]") {
    std::vector<frequency_entry> entries =
        get_frequency_entries_from_file("kanji_frequency_list.csv");

    pugi::xml_document kanjidic2_doc {};
    pugi::xml_parse_result result_kanjidic2 =
        kanjidic2_doc.load_file("kanjidic2.xml");
    REQUIRE(result_kanjidic2);

    pugi::xml_document jmdict_e_doc {};
    pugi::xml_parse_result result_jmdict_e =
        jmdict_e_doc.load_file("JMdict_e.xml");
    REQUIRE(result_jmdict_e);

    std::filesystem::remove_all(out_dir);
    REQUIRE(!std::filesystem::exists(out_dir));
    REQUIRE(std::filesystem::create_directory(out_dir));

    std::mutex mutex {};
    std::vector<std::thread> threads {};
    int seq = 1;

    for (int i = 0; i < num_threads; i++) {
      std::thread thread =
          std::thread([&mutex, &seq, &entries,
                       &kanjidic2_doc = std::as_const(kanjidic2_doc),
                       &jmdict_e_doc = std::as_const(jmdict_e_doc)]() {
            while (true) {

                std::optional<int> thread_seq = std::nullopt;
                std::optional<frequency_entry> thread_frequency_entry = std::nullopt;
                {
                    std::scoped_lock lock(mutex);

                    if (entries.empty()) {
                        return;
                    }
                    thread_seq = seq;
                    seq++;
                    thread_frequency_entry = *entries.erase(entries.begin());
                }

                kanji_data kanji_data {
                    thread_frequency_entry.value().get_kanji(),
                    static_cast<uint16_t>(thread_seq.value()),
                    kanjidic2_doc,
                    jmdict_e_doc
                };

                std::string path = create_card(kanji_data, out_dir);
                std::cout << "Created: " << path << std::endl;
            }
        });
        threads.push_back(std::move(thread));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
}
