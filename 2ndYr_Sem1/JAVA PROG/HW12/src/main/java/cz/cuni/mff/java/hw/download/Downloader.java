package cz.cuni.mff.java.hw.download;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Downloader {
    public static void downloader(String[] args) {
        if (args.length != 1) {
            System.out.print("Error");
            return;
        }

        String urlString = args[0];

        try {
            URL url = new URL(urlString);

            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");

            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));

            StringBuilder siteContent = new StringBuilder();
            String line;
            while ((line = in.readLine()) != null) {
                siteContent.append(line);
            }

            in.close();

            String html = siteContent.toString();
            Pattern imagePattern = Pattern.compile("<img\\s+src=\"([^\"]+)\"");
            int totalSize = getTotalSize(imagePattern, html, url);

            System.out.print(totalSize);
        }
        catch (Exception e) {
            System.out.print("Error");
        }
    }

    private static int getTotalSize(Pattern imagePattern, String html, URL url) throws IOException {
        Matcher matcher = imagePattern.matcher(html);

        int totalSize = 0;
        HashSet<String> imageUrls = new HashSet<>();

        while (matcher.find()) {
            String imageUrl = matcher.group(1);
            if (!imageUrl.startsWith("http")) {
                imageUrl = new URL(url, imageUrl).toString();
            }

            if (!imageUrls.contains(imageUrl)) {
                imageUrls.add(imageUrl);

                URL imgUrl = new URL(imageUrl);
                HttpURLConnection imageConnection = (HttpURLConnection) imgUrl.openConnection();
                imageConnection.setRequestMethod("GET");
                totalSize += imageConnection.getContentLength();
                imageConnection.disconnect();
            }
        }
        return totalSize;
    }
}
