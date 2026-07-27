package org.example.features.search;

import net.serenitybdd.junit.runners.SerenityParameterizedRunner;
import net.thucydides.core.annotations.Managed;
import net.thucydides.core.annotations.Steps;
import net.thucydides.junit.annotations.UseTestDataFrom;
import net.thucydides.junit.annotations.Qualifier;
import org.example.steps.serenity.EmagSteps;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.openqa.selenium.WebDriver;

@RunWith(SerenityParameterizedRunner.class)
@UseTestDataFrom("src/test/resources/EmagSearchData.csv")
public class EmagSearchDDT {

    @Managed(uniqueSession = true)
    public WebDriver webdriver;

    @Steps
    public EmagSteps user;

    public String keyword;
    public String expectedMessage;

    @Qualifier
    public String getQualifier() {
        return keyword;
    }

    @Test
    public void search_functionality_test() {
        user.is_on_the_home_page();
        user.searches_for_product(keyword);
        user.should_see_search_result_message(expectedMessage);
    }
}