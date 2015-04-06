#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void apply(Mat & dst, const Mat & src, Point orig, Point motion)
{
    Point2f srcTri[] = { { 0.f, 0.f }, { 0.f, static_cast<float>(src.rows - 1) }, orig };
    Point2f dstTri[] = { { 0.f, 0.f }, { 0.f, static_cast<float>(src.rows - 1) }, motion };

    Mat m = getAffineTransform(srcTri, dstTri);

    warpAffine(src, dst, m, dst.size());
}

 int main(int argc, char** argv)
 {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    Mat src, dst;

    src = imread(argv[1], 1);
    dst = src.clone();
    IplImage img = dst;

    SDL_Window *wnd = SDL_CreateWindow("Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, img.width, img.height, 0);
    SDL_Renderer *rdr = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(rdr, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, img.width, img.height);

    Point2f orig{ 1.f, 1.f };
    Point2f motion{ 1.f, 1.f };
    bool pressed = false;

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYUP:
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_q:    goto end; break;
                    }

                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        orig = Point{event.button.x, event.button.y};
                        motion = orig;
                        pressed = true;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        orig = { 1.f, 1.f };
                        motion = { 1.f, 1.f };
                        pressed = false;
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                {
                    if (pressed)
                    {
                        motion = Point{event.motion.x, event.motion.y};
                    }
                }
                break;

            case SDL_QUIT:
                goto end;
                break;
            }
        }

        apply(dst, src, orig, motion);

        int8_t *pixels; int pitch;
        SDL_LockTexture(texture, nullptr, (void **)&pixels, &pitch);
        memcpy(pixels, img.imageData, img.imageSize);
        SDL_UnlockTexture(texture);

        SDL_RenderClear(rdr);
        SDL_RenderCopy(rdr, texture, nullptr, nullptr);
        SDL_RenderPresent(rdr);

        SDL_Delay(50);
    }

end:
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(wnd);

    SDL_Quit();

    return 0;
  }