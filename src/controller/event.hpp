#pragma once

namespace leo {
  namespace controller {

    enum Event {
      DRAWABLE_ADDED = 0,
      DRAWABLE_MODIFIED = 1 << 0,
      DRAWABLE_DELETED = 1 << 1
    };

  }
}
