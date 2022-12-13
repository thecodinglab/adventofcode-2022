local M = {}

local function find_executable()
  local file = vim.api.nvim_buf_get_name(0)
  return file:gsub('cpp$', 'bin')
end

local function build_executable(exe)
  local cwd = vim.fn.getcwd()
  local target = exe:sub(cwd:len() + 2)

  return os.execute('make -C "' .. cwd .. '" "' .. target .. '"')
end

local function debug_executable()
  local exe = find_executable()
  if build_executable(exe) then
    print('build successful for ' .. exe)
    return exe
  end
end

function M.setup()
  local dap = require('dap')

  dap.configurations.cpp = {
    {
      name = 'debug sample',
      type = 'lldb',
      request = 'launch',
      program = debug_executable,
      cwd = '${fileDirname}',
      args = { 'sample.txt' },
      stopOnEntry = false,
    },
    {
      name = 'debug input',
      type = 'lldb',
      request = 'launch',
      program = debug_executable,
      cwd = '${fileDirname}',
      args = { 'input.txt' },
      stopOnEntry = false,
    },
  }
end

return M
