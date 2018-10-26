/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package puzzleblock;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

/**
 *
 * @author khant
 */
public class Effect {
    /**
     * Converts <a>sun.awt.image</a> to <a>java.awt.BufferedImage</a>.
     * @param img Image
     * @return  BufferedImage for manipulation
     */
    public static BufferedImage toBufferedImage(Image img) {
        BufferedImage bi = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2 = bi.createGraphics();
        g2.drawImage(img, 0, 0, null);
        g2.dispose();
        return bi;
    } 
    
    /**
     * Changes the hue, saturation and luminance of a <code>BufferedImage</code> by reading the color code and 
     * manipulating every single pixel of the it.
     * 
     * @param img BufferedImage to be manipulated
     * @param h Hue
     * @param s Saturation
     * @param l Luminance
     *
     * @return BufferedImage with new HSL values
     */
    public static BufferedImage imageHSL_manipulation(BufferedImage img, float h, float s, float l) {
        int width = img.getWidth();
        int height = img.getHeight();
        BufferedImage newImg = new BufferedImage(width, height, img.getType());
        for (int x=0; x<width; x++) {
            for (int y=0; y<height; y++) {
                int rgb = img.getRGB(x, y);
                rgb = colorHSL_manipulation(rgb, h, s, l);
                newImg.setRGB(x, y, rgb);
            }
        }
        return newImg;
    }

    /**
     * Output the color with new values depending on the color given and levels of HSL increment.
     * 
     * @param rgb Color code to be manipulated
     * @param h Hue
     * @param s Saturation
     * @param l Luminance
     * 
     * @return New color code
     */
    private static int colorHSL_manipulation(int rgb, float h, float s, float l) {
        int A = (rgb >> 24) & 0xFF;
        int R = (rgb >> 16) & 0xFF;
        int G = (rgb >> 8) & 0xFF;
        int B = rgb & 0xFF;
        float[] hslVals = Color.RGBtoHSB(R, G, B, null);
        float H = hslVals[0];
        float S = hslVals[1];
        float L = hslVals[2];
        H = (1 + H + (h / 360)) % 1;
        S = ValueContrastBuffer(S, s);
        L = ValueContrastBuffer(L, l);
        rgb = Color.HSBtoRGB(H, S, L);
        rgb = (A << 24) + (((rgb >> 16) & 0xFF) << 16) + (((rgb >> 8) & 0xFF) << 8) + (rgb & 0xFF);
        return rgb;
    }

    /**
     * @param val This value may be saturation or brightness
     * @param lvl Level of increment
     * 
     * @return Buffered value
     */
    private static float ValueContrastBuffer(float val, float lvl) {
        float posIncr = Math.abs((1 - val) * lvl * 0.01f);
        float negIncr = Math.abs(val * lvl * 0.01f);
        if (lvl > 0) {
            if (posIncr > negIncr)
                posIncr -= (posIncr - negIncr) * 0.6f;
            return val + posIncr;
        }
        else if (lvl < 0) {
            if (negIncr > posIncr)
                negIncr -= (negIncr - posIncr) * 0.6f;
            return val - negIncr;}
        else
            return val;
    }
    
    /**
     * The function that takes the rendering hints for good graphics of an image before calling the image processing codes.
     * 
     * @param g2 Graphics of an image
     * @param processor  Block of codes for drawing
     */
    public static void drawRenderedImage(Graphics2D g2, ImageManipulator processor) {
        g2.setRenderingHint(RenderingHints.KEY_ALPHA_INTERPOLATION, RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY);
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2.setRenderingHint(RenderingHints.KEY_COLOR_RENDERING, RenderingHints.VALUE_COLOR_RENDER_QUALITY);
        g2.setRenderingHint(RenderingHints.KEY_DITHERING, RenderingHints.VALUE_DITHER_ENABLE);
        g2.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_ON);
        g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
        g2.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);

        processor.Manipulate(g2);
        g2.dispose();
    }

    /**
     * The function which scales the image size to fit the given width and height with fine quality.
     * 
     * @param img Image to be manipulated
     * @param width Given width
     * @param height Given height
     * @return Scaled image
     */
    public static BufferedImage resize(BufferedImage img, int width, int height) {
        BufferedImage newImg = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2 = newImg.createGraphics();
        drawRenderedImage(g2, (g) -> {
            g.drawImage(img, 0, 0, width, height, null);
        });
        return newImg;
    }
}

/**
 * The interface for drawing processes.
 * 
 * @author khant
 */
interface ImageManipulator {
    public void Manipulate(Graphics2D g2);
}
