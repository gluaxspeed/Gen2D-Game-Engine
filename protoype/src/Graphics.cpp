#include "Graphics.hpp"

Graphics *Graphics::sInstance = NULL;
bool Graphics::sInitialized = false;

Graphics* Graphics::Instance() {
  
  if(!sInstance) {
    sInstance = new Graphics();
  }

  return sInstance;
}

void Graphics::Release() {
  
  delete sInstance;
  sInstance = NULL;

  sInitialized = false;
}

bool Graphics::Initialized() {
  
  return sInitialized;
}

Graphics::Graphics() {
  
  mBackBuffer = NULL;

  sInitialized = Init();
}

Graphics::~Graphics() {
  
  SDL_DestroyWindow(mWindow);
  mWindow = NULL;

  SDL_DestroyRenderer(mRenderer);
  mRenderer = NULL;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

bool Graphics::Init() {
  
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL Init Error: %s\n", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if(!mWindow) {
    printf("Window Creation Error: %s\n", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
  if(!mRenderer) {

    printf("Renderer Creation Error: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawColor(mRenderer, 0xff, 0xff, 0xff, 0xff);

  int flags = IMG_INIT_PNG;
  if(!(IMG_Init(flags) & flags)) {

    printf("Renderer Init Error: %s\n", IMG_GetError());
    return false;
  }

  if(TTF_Init() == -1) {

    printf("TTF Init Error: %s\n", TTF_GetError());
    return false;
  }

  mBackBuffer = SDL_GetWindowSurface(mWindow);

  return true;
}

SDL_Texture* Graphics::LoadTexture(std::string path) {

  SDL_Texture *tex = NULL;

  SDL_Surface *surface = IMG_Load(path.c_str());
  if(!surface) {

    printf("Image Load Error: Path(%s) - Error(%s)\n", path.c_str(), IMG_GetError());
    return tex;
  }

  tex = SDL_CreateTextureFromSurface(mRenderer, surface);
  if(!tex) {

    printf("Create Texture Error:%s\n", SDL_GetError());
    return tex;
  }

  SDL_FreeSurface(surface);

  return tex;
}

SDL_Texture* Graphics::CreateTextTexture(TTF_Font *font, std::string  text, SDL_Color color) {

  SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
  if(!surface) {

    printf("Text Render Error: %s\n", TTF_GetError());
    return NULL;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(mRenderer, surface);
  if(!tex) {

    printf("Create Texture Error:%s\n", SDL_GetError());
    return NULL;
  }

  SDL_FreeSurface(surface);

  return tex;
}

void Graphics::ClearBackBuffer() {

  SDL_RenderClear(mRenderer);
}

void Graphics::DrawTexture(SDL_Texture *tex, SDL_Rect *clip, SDL_Rect *rend, float angle, SDL_RendererFlip flip) {

  SDL_RenderCopyEx(mRenderer, tex, clip, rend, angle, NULL, flip);
}

void Graphics::Render() {
  
  SDL_RenderPresent(mRenderer);
}
