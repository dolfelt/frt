// sdl2_adapter.h
/*
  FRT - A Godot platform targeting single board computers
  Copyright (c) 2017-2021  Emanuele Fornara
  SPDX-License-Identifier: MIT
 */

namespace frt {

struct KeyMap {
	int sdl2_code;
	int gd_code;
} keymap[] = {
	{ SDLK_SPACE, ' ' },
	{ SDLK_a, 'A' },
	{ SDLK_b, 'B' },
	{ SDLK_c, 'C' },
	{ SDLK_d, 'D' },
	{ SDLK_e, 'E' },
	{ SDLK_f, 'F' },
	{ SDLK_g, 'G' },
	{ SDLK_h, 'H' },
	{ SDLK_i, 'I' },
	{ SDLK_j, 'J' },
	{ SDLK_k, 'K' },
	{ SDLK_l, 'L' },
	{ SDLK_m, 'M' },
	{ SDLK_n, 'N' },
	{ SDLK_o, 'O' },
	{ SDLK_p, 'P' },
	{ SDLK_q, 'Q' },
	{ SDLK_r, 'R' },
	{ SDLK_s, 'S' },
	{ SDLK_t, 'T' },
	{ SDLK_u, 'U' },
	{ SDLK_v, 'V' },
	{ SDLK_w, 'W' },
	{ SDLK_x, 'X' },
	{ SDLK_y, 'Y' },
	{ SDLK_z, 'Z' },
	{ SDLK_0, '0' },
	{ SDLK_1, '1' },
	{ SDLK_2, '2' },
	{ SDLK_3, '3' },
	{ SDLK_4, '4' },
	{ SDLK_5, '5' },
	{ SDLK_6, '6' },
	{ SDLK_7, '7' },
	{ SDLK_8, '8' },
	{ SDLK_9, '9' },
	{ SDLK_F1, KEY_F1 },
	{ SDLK_F2, KEY_F2 },
	{ SDLK_F3, KEY_F3 },
	{ SDLK_F4, KEY_F4 },
	{ SDLK_F5, KEY_F5 },
	{ SDLK_F6, KEY_F6 },
	{ SDLK_F7, KEY_F7 },
	{ SDLK_F8, KEY_F8 },
	{ SDLK_F9, KEY_F9 },
	{ SDLK_F10, KEY_F10 },
	{ SDLK_F11, KEY_F11 },
	{ SDLK_F12, KEY_F12 },
	{ SDLK_UP, KEY_UP },
	{ SDLK_DOWN, KEY_DOWN },
	{ SDLK_LEFT, KEY_LEFT },
	{ SDLK_RIGHT, KEY_RIGHT },
	{ SDLK_TAB, KEY_TAB },
	{ SDLK_BACKSPACE, KEY_BACKSPACE },
	{ SDLK_INSERT, KEY_INSERT },
	{ SDLK_DELETE, KEY_DELETE },
	{ SDLK_HOME, KEY_HOME },
	{ SDLK_END, KEY_END },
	{ SDLK_PAGEUP, KEY_PAGEUP },
	{ SDLK_PAGEDOWN, KEY_PAGEDOWN },
	{ SDLK_RETURN, KEY_RETURN },
	{ SDLK_ESCAPE, KEY_ESCAPE },
	{ SDLK_LCTRL, KEY_CONTROL },
	{ SDLK_RCTRL, KEY_CONTROL },
	{ SDLK_LALT, KEY_ALT },
	{ SDLK_RALT, KEY_ALT },
	{ SDLK_LSHIFT, KEY_SHIFT },
	{ SDLK_RSHIFT, KEY_SHIFT },
	{ SDLK_LGUI, KEY_META },
	{ SDLK_RGUI, KEY_META },
	{ SDLK_KP_0, KEY_KP_0 },
	{ SDLK_KP_1, KEY_KP_1 },
	{ SDLK_KP_2, KEY_KP_2 },
	{ SDLK_KP_3, KEY_KP_3 },
	{ SDLK_KP_4, KEY_KP_4 },
	{ SDLK_KP_5, KEY_KP_5 },
	{ SDLK_KP_6, KEY_KP_6 },
	{ SDLK_KP_7, KEY_KP_7 },
	{ SDLK_KP_8, KEY_KP_8 },
	{ SDLK_KP_9, KEY_KP_9 },
	{ SDLK_KP_MULTIPLY, KEY_KP_MULTIPLY },
	{ SDLK_KP_MINUS, KEY_KP_SUBTRACT },
	{ SDLK_KP_PLUS, KEY_KP_ADD },
	{ SDLK_KP_PERIOD, KEY_KP_PERIOD },
	{ SDLK_KP_ENTER, KEY_KP_ENTER },
	{ SDLK_KP_DIVIDE, KEY_KP_DIVIDE },
	{ 0, 0 },
};

struct SDL2EventHandler {
	virtual ~SDL2EventHandler();
	virtual void handle_key_event(int gd_code, bool pressed) = 0;
	virtual void handle_quit_event() = 0;
};

SDL2EventHandler::~SDL2EventHandler() {
}

class SDL2OS {
private:
	SDL_Window *window_;
	SDL_GLContext context_;
	SDL2EventHandler *handler_;
	InputModifierState st_;
	void key_event(const SDL_Event &ev) {
		st_.shift = ev.key.keysym.mod & KMOD_SHIFT;
		st_.alt = ev.key.keysym.mod & KMOD_ALT;
		st_.control = ev.key.keysym.mod & KMOD_CTRL;
		st_.meta = ev.key.keysym.mod & KMOD_GUI;
		if (ev.key.repeat)
			return;
		bool pressed = ev.key.state == SDL_PRESSED;
		for (int i = 0; keymap[i].sdl2_code; i++) {
			if (keymap[i].sdl2_code == ev.key.keysym.sym) {
				handler_->handle_key_event(keymap[i].gd_code, pressed);
				return;
			}
		}
	}
public:
	SDL2OS(SDL2EventHandler *handler) : handler_(handler) {
		st_.shift = false;
		st_.alt = false;
		st_.control = false;
		st_.meta = false;
	}
	void init(int width, int height) {
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			fatal("SDL_Init failed.");
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		if (!(window_ = SDL_CreateWindow("frt2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI)))
			fatal("SDL_CreateWindow failed.");
		context_ = SDL_GL_CreateContext(window_);
		SDL_GL_MakeCurrent(window_, context_);
		frt_resolve_symbols_gles2(SDL_GL_GetProcAddress);
	}
	void cleanup() {
		SDL_DestroyWindow(window_);
		SDL_Quit();
	}
	void make_current() {
		SDL_GL_MakeCurrent(window_, context_);
	}
	void release_current() {
		// TODO: add release
	}
	void swap_buffers() {
		SDL_GL_SwapWindow(window_);
	}
	void dispatch_events() {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				key_event(ev);
				break;
			case SDL_QUIT:
				//handler_->handle_quit_event(); // audio bug
				exit(1);
				break;
			}
		}
	}
	void get_modifier_state(InputModifierState &state) const {
		state = st_;
	}
};

} // namespace frt
