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
@UseTestDataFrom("src/test/resources/EmagFilterData.csv")
public class EmagFilterDDT {

    @Managed(uniqueSession = true)
    public WebDriver webdriver;

    @Steps
    public EmagSteps user;

    public String keyword;
    public String filterName;
    public String expectedMessage;

    @Qualifier
    public String getQualifier() {
        return keyword + " -> " + filterName;
    }

    @Test
    public void filter_functionality_test() {
        user.is_on_the_home_page();
        user.searches_for_product(keyword);
        user.filters_by_brand(filterName);
        user.should_see_results_containing(expectedMessage);
    }
}