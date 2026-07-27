package org.example.steps.serenity;

import net.thucydides.core.annotations.Step;
import org.example.pages.EmagPage;
import static org.junit.Assert.assertTrue;

public class EmagSteps {

    EmagPage emagPage;

    @Step
    public void is_on_the_home_page() {
        emagPage.open();
    }

    @Step
    public void searches_for_product(String keyword) {
        emagPage.searchFor(keyword);
    }

    @Step
    public void should_see_search_result_message(String expectedMessage) {
        String actualMessage = emagPage.getSearchResultMessage(expectedMessage);

        assertTrue("Expected search results to contain: '" + expectedMessage + "', but got: '" + actualMessage + "'",
                actualMessage.toLowerCase().contains(expectedMessage.toLowerCase()));
    }

    @Step
    public void filters_by_brand(String filterName) {
        emagPage.applyFilter(filterName);
    }

    @Step
    public void should_see_results_containing(String expectedMessage) {
        String actualMessage = emagPage.getSearchResultMessage(expectedMessage);

        assertTrue("Expected filtered results to contain: '" + expectedMessage + "', but got: '" + actualMessage + "'",
                actualMessage.toLowerCase().contains(expectedMessage.toLowerCase()));
    }
}