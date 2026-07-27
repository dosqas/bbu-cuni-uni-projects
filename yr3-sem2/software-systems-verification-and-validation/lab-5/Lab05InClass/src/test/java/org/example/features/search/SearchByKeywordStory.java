package org.example.features.search;

import net.serenitybdd.junit.runners.SerenityRunner;
import net.thucydides.core.annotations.Issue;
import net.thucydides.core.annotations.Managed;
import net.thucydides.core.annotations.Pending;
import net.thucydides.core.annotations.Steps;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.openqa.selenium.WebDriver;

import org.example.steps.serenity.EndUserSteps;

@RunWith(SerenityRunner.class)
public class SearchByKeywordStory {

    @Managed(uniqueSession = true)
    public WebDriver webdriver;

    @Steps
    public EndUserSteps anna;

    @Issue("#WIKI-1")
    @Test
    public void searching_by_keyword_apple_should_display_the_corresponding_article() {
        anna.is_the_home_page();
        anna.looks_for("apple");
        anna.should_see_definition("A common, firm, round fruit produced by a tree of the genus Malus.");

    }

    @Test
    public void searching_by_keyword_banana_should_display_the_corresponding_article() {
        anna.is_the_home_page();
        anna.looks_for("pear");
        anna.should_see_definition("An edible fruit produced by the pear tree, similar to an apple but typically elongated towards the stem.");
    }

    @Pending @Test
    public void searching_by_ambiguious_keyword_should_display_the_disambiguation_page() {
    }

    @Test
    public void searching_by_keyword_software_should_display_the_corresponding_article() {
        anna.is_the_home_page();
        anna.looks_for("software");
        anna.should_see_definition("Encoded computer instructions, usually modifiable (unless stored in some form of unalterable memory such as ROM).");
    }

    @Test
    public void searching_by_keyword_testing_should_display_the_corresponding_article() {
        anna.is_the_home_page();
        anna.looks_for("test");
        anna.should_see_definition("A challenge, trial.");
    }
} 