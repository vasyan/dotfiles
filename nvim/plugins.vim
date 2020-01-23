call plug#begin()

if has('nvim')
  Plug 'Shougo/denite.nvim', { 'do': ':UpdateRemotePlugins' }
else
  Plug 'Shougo/denite.nvim'
  Plug 'roxma/nvim-yarp'
  Plug 'roxma/vim-hug-neovim-rpc'
endif

" Use release branch (Recommend)
Plug 'neoclide/coc.nvim', {'branch': 'release'}

" Or build from source code by use yarn: https://yarnpkg.com
Plug 'neoclide/coc.nvim', {'do': 'yarn install --frozen-lockfile'}

Plug 'leafgarland/typescript-vim'

Plug 'preservim/nerdtree'

Plug 'preservim/nerdcommenter'

Plug 'vim-airline/vim-airline'

Plug 'vim-airline/vim-airline-themes'

Plug 'easymotion/vim-easymotion'

Plug 'ianks/vim-tsx'

Plug 'rafi/awesome-vim-colorschemes'

Plug 'airblade/vim-gitgutter'

Plug 'tpope/vim-fugitive'

Plug 'Shougo/neosnippet'

Plug 'Shougo/neosnippet-snippets'

call plug#end()

