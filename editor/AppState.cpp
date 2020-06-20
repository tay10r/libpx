#include "AppState.hpp"

#include "App.hpp"

namespace px {

Platform* AppState::getPlatform() noexcept
{
  return app->getPlatform();
}

MenuBar* AppState::getMenuBar() noexcept
{
  return app->getMenuBar();
}

const MenuBar* AppState::getMenuBar() const noexcept
{
  return app->getMenuBar();
}

Log* AppState::getLog() noexcept
{
  return app->getLog();
}

const Log* AppState::getLog() const noexcept
{
  return app->getLog();
}

} // namespace px
